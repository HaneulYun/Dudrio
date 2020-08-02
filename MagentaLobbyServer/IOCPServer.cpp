#include "pch.h"
#include "IOCPServer.h"

IOCPServer::IOCPServer()
{
	server_worker_run = true;
	server_accept_run = true;
	client_worker_run = true;
	client_accept_run = true;
}

IOCPServer::~IOCPServer()
{
	for (auto s : g_servers)
		disconnect_server(s.first);

	for (auto cl : g_users)
		disconnect_client(cl.first);

	WSACleanup();
	destroy_threads();
}

void IOCPServer::init_server()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	l_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	// bind -------------------------------------------------------
	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_TO_LOBBY_SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));

	// listen -----------------------------------------------------
	listen(l_socket, SOMAXCONN);

	int option = TRUE;
	setsockopt(l_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

	/////////////////////////////////////////////////////////////////////////////////////////
	cl_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	// bind -------------------------------------------------------
	SOCKADDR_IN c_address;
	memset(&c_address, 0, sizeof(c_address));
	c_address.sin_family = AF_INET;
	c_address.sin_port = htons(CLIENT_TO_LOBBY_SERVER_PORT);
	c_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(cl_socket, reinterpret_cast<sockaddr*>(&c_address), sizeof(c_address));

	// listen -----------------------------------------------------
	listen(cl_socket, SOMAXCONN);

	int coption = TRUE;
	setsockopt(cl_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&coption, sizeof(coption));
}

void IOCPServer::start_server()
{
	init_clients();

	g_ciocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);// NUM_OF_CPU * 2 + 1);
	
	create_worker_threads();
	create_accept_threads();

	cout << "Start_Server Complete" << endl;
}

void IOCPServer::init_clients()
{
	for (auto& cl : g_servers)
		delete cl.second;
	g_servers.clear();

	for (auto& cl : g_users)
		delete cl.second;
	g_users.clear();

	g_servers.reserve(L_MAX_SERVER);
	g_users.reserve(L_MAX_USER);
}

// thread ---------------------------------
void IOCPServer::create_worker_threads()
{
	//worker_threads.reserve(NUM_OF_CPU * 2 + 2);
	//for (int i = 0; i < NUM_OF_CPU * 2 + 1; ++i) {
		server_worker_threads.emplace_back([this]() {server_worker_thread_loop(); });
	//}
		client_worker_threads.emplace_back([this]() {client_worker_thread_loop(); });

	cout << "Create Worker_Threads Complete" << endl;
}

void IOCPServer::create_accept_threads()
{
	client_accept_thread = thread([this]() { client_accept_thread_loop(); });
	server_accept_thread = thread([this]() { server_accept_thread_loop(); });

	cout << "Create Accept_Thread Complete" << endl;
}

void IOCPServer::destroy_threads()
{
	server_worker_run = false;
	CloseHandle(g_iocp);
	for (auto& w_t : server_worker_threads)
		if (w_t.joinable())
			w_t.join();
	
	server_accept_run = false;
	closesocket(l_socket);
	
	if (server_accept_thread.joinable())
		server_accept_thread.join();

	///////////////////////////////////

	client_worker_run = false;
	CloseHandle(g_ciocp);
	for (auto& w_t : client_worker_threads)
		if (w_t.joinable())
			w_t.join();

	client_accept_run = false;
	closesocket(cl_socket);

	if (client_accept_thread.joinable())
		client_accept_thread.join();
}

void IOCPServer::server_worker_thread_loop()
{
	while (server_worker_run) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);

		if (g_servers.count(user_id) == 0)
			continue;

		switch (exover->op) {
		case OP_RECV:
			if (0 == io_byte)
				disconnect_server(user_id);
			else {
				server_recv_packet_construct(user_id, io_byte);
				ZeroMemory(&g_servers[user_id]->m_recv_over.over, sizeof(g_servers[user_id]->m_recv_over.over));
				DWORD flags = 0;
				WSARecv(g_servers[user_id]->m_s, &g_servers[user_id]->m_recv_over.wsabuf, 1, NULL, &flags, &g_servers[user_id]->m_recv_over.over, NULL);
			}
			break;
		case OP_SEND:
			if (0 == io_byte)
				disconnect_server(user_id);
			delete exover;
			break;
		default:
			std::cout << "Invalid Operation " << exover->op << endl;
			break;
		}
	}
}

void IOCPServer::client_worker_thread_loop()
{
	while (client_worker_run) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_ciocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);

		if (g_users.count(user_id) == 0)
			continue;

		switch (exover->op) {
		case OP_RECV:
			if (0 == io_byte)
				disconnect_client(user_id);
			else {
				client_recv_packet_construct(user_id, io_byte);
				ZeroMemory(&g_users[user_id]->m_recv_over.over, sizeof(g_users[user_id]->m_recv_over.over));
				DWORD flags = 0;
				WSARecv(g_users[user_id]->m_s, &g_users[user_id]->m_recv_over.wsabuf, 1, NULL, &flags, &g_users[user_id]->m_recv_over.over, NULL);
			}
			break;
		case OP_SEND:
			if (0 == io_byte)
				disconnect_client(user_id);
			delete exover;
			break;
		default:
			std::cout << "Invalid Operation " << exover->op << endl;
			break;
		}
	}
}

void IOCPServer::server_accept_thread_loop()
{
	SOCKADDR_IN client_addr;
	auto client_len = static_cast<int>(sizeof(client_addr));

	while (server_accept_run)
	{
		// 접속 받을 유저 소켓을 생성 한다.
		int idx = 0;

		//g_clients_lock.lock();
		while (idx < L_MAX_SERVER) {
			if (g_servers.count(idx) == 0) {
				cout << "New server " << idx << " is generated" << endl;
				g_servers[idx] = new Client(idx);
				g_servers[idx]->m_status = ST_ALLOC;
				g_servers[idx]->o_type = O_SERVER;
				break;
			}
			else {
				idx++;
			}
		}
		//g_clients_lock.unlock();

		if (idx >= L_MAX_SERVER)
			continue;

		lock_guard<mutex>lock_guard_client(g_servers[idx]->m_cl);
		ZeroMemory(&g_servers[idx]->m_recv_over.over, sizeof(g_servers[idx]->m_recv_over.over));
		g_servers[idx]->m_s = WSAAccept(l_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len, NULL, NULL);
		if (INVALID_SOCKET == g_servers[idx]->m_s)	continue;

		bool retval = CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_servers[idx]->m_s), g_iocp, idx, 0);
		if (false == retval) {
			//lock_guard<mutex>lock_guard(g_clients_lock);
			cout << "Bind IO Completion Port Error" << GetLastError() << endl;
			closesocket(g_servers[idx]->m_s);
			delete g_servers[idx];
			g_servers.erase(idx);
			continue;
		}

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);

		DWORD flags = 0;
		g_servers[idx]->m_recv_over.c_socket = g_servers[idx]->m_s;
		g_servers[idx]->m_recv_over.op = OP_RECV;
		retval = WSARecv(g_servers[idx]->m_s, &g_servers[idx]->m_recv_over.wsabuf, 1, NULL, &flags, &g_servers[idx]->m_recv_over.over, NULL);

		if (retval == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
			//lock_guard<mutex>lock_guard(g_clients_lock);
			cout << "Bind Recv Operation Error" << WSAGetLastError() << endl;
			closesocket(g_servers[idx]->m_s);
			delete g_servers[idx];
			g_servers.erase(idx);
			continue;
		}
	}
}

void IOCPServer::client_accept_thread_loop()
{
	SOCKADDR_IN client_addr;
	auto client_len = static_cast<int>(sizeof(client_addr));

	while (client_accept_run)
	{
		// 접속 받을 유저 소켓을 생성 한다.
		int idx = 0;

		//g_clients_lock.lock();
		while (idx < L_MAX_USER) {
			if (g_users.count(idx) == 0) {
				cout << "New client " << idx << " is generated" << endl;
				g_users[idx] = new Client(idx);
				g_users[idx]->m_status = ST_ALLOC;
				break;
			}
			else {
				idx++;
			}
		}
		//g_clients_lock.unlock();

		if (idx >= L_MAX_USER)
			continue;

		lock_guard<mutex>lock_guard_client(g_users[idx]->m_cl);
		ZeroMemory(&g_users[idx]->m_recv_over.over, sizeof(g_users[idx]->m_recv_over.over));
		g_users[idx]->m_s = WSAAccept(cl_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len, NULL, NULL);
		if (INVALID_SOCKET == g_users[idx]->m_s)	continue;

		bool retval = CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_users[idx]->m_s), g_ciocp, idx, 0);
		if (false == retval) {
			//lock_guard<mutex>lock_guard(g_clients_lock);
			cout << "Bind IO Completion Port Error" << GetLastError() << endl;
			closesocket(g_users[idx]->m_s);
			delete g_users[idx];
			g_users.erase(idx);
			continue;
		}

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);

		DWORD flags = 0;
		g_users[idx]->m_recv_over.c_socket = g_users[idx]->m_s;
		g_users[idx]->m_recv_over.op = OP_RECV;
		retval = WSARecv(g_users[idx]->m_s, &g_users[idx]->m_recv_over.wsabuf, 1, NULL, &flags, &g_users[idx]->m_recv_over.over, NULL);

		if (retval == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
			//lock_guard<mutex>lock_guard(g_clients_lock);
			cout << "Bind Recv Operation Error" << WSAGetLastError() << endl;
			closesocket(g_users[idx]->m_s);
			delete g_users[idx];
			g_users.erase(idx);
			continue;
		}
	}
}

void IOCPServer::server_recv_packet_construct(int user_id, int io_byte)
{
	EXOVER& r_o = g_servers[user_id]->m_recv_over;

	// 나머지 바이트
	int rest_byte = io_byte;
	// 처리할 데이터의 포인터
	char* p = r_o.io_buf;
	int packet_size = 0;
	// 미리 받아논 게 있다면?
	if (0 != g_servers[user_id]->m_prev_size)	packet_size = (unsigned char)g_servers[user_id]->m_packet_buf[0];
	// 처리할 데이터가 남아있다면?
	while (rest_byte > 0)
	{
		// 우리가 처리해야 되는 패킷을 전에 처리해본 적이 없을 때
		// == 패킷의 시작 부분이 있다
		if (0 == packet_size)	packet_size = (unsigned char)*p;
		// 패킷을 완성할 수 있다
		if (packet_size <= rest_byte + g_servers[user_id]->m_prev_size) {
			memcpy(g_servers[user_id]->m_packet_buf + g_servers[user_id]->m_prev_size, p, packet_size - g_servers[user_id]->m_prev_size);
			p += packet_size - g_servers[user_id]->m_prev_size;
			rest_byte -= packet_size - g_servers[user_id]->m_prev_size;
			packet_size = 0;	// 이 패킷은 처리되었다
			process_packet(user_id, g_servers[user_id]->m_packet_buf);
			g_servers[user_id]->m_prev_size = 0;
		}
		else // 완성할 수 없다
		{
			// 저장하여 다음 번에 재조립 해줘야 함
			memcpy(g_servers[user_id]->m_packet_buf + g_servers[user_id]->m_prev_size, p, rest_byte);
			g_servers[user_id]->m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void IOCPServer::client_recv_packet_construct(int user_id, int io_byte)
{
	EXOVER& r_o = g_users[user_id]->m_recv_over;

	// 나머지 바이트
	int rest_byte = io_byte;
	// 처리할 데이터의 포인터
	char* p = r_o.io_buf;
	int packet_size = 0;
	// 미리 받아논 게 있다면?
	if (0 != g_users[user_id]->m_prev_size)	packet_size = (unsigned char)g_users[user_id]->m_packet_buf[0];
	// 처리할 데이터가 남아있다면?
	while (rest_byte > 0)
	{
		// 우리가 처리해야 되는 패킷을 전에 처리해본 적이 없을 때
		// == 패킷의 시작 부분이 있다
		if (0 == packet_size)	packet_size = (unsigned char)*p;
		// 패킷을 완성할 수 있다
		if (packet_size <= rest_byte + g_users[user_id]->m_prev_size) {
			memcpy(g_users[user_id]->m_packet_buf + g_users[user_id]->m_prev_size, p, packet_size - g_users[user_id]->m_prev_size);
			p += packet_size - g_users[user_id]->m_prev_size;
			rest_byte -= packet_size - g_users[user_id]->m_prev_size;
			packet_size = 0;	// 이 패킷은 처리되었다
			process_packet(user_id, g_users[user_id]->m_packet_buf);
			g_users[user_id]->m_prev_size = 0;
		}
		else // 완성할 수 없다
		{
			// 저장하여 다음 번에 재조립 해줘야 함
			memcpy(g_users[user_id]->m_packet_buf + g_users[user_id]->m_prev_size, p, rest_byte);
			g_users[user_id]->m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void IOCPServer::send_packet_server(int user_id, void* p)
{
	unsigned char* buf = reinterpret_cast<unsigned char*>(p);

	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);

	// IpBuffers 항목에 u의 wsabuf은 이미 Recv에서 쓰고 있기 때문에 사용하면 안됨
	WSASend(g_servers[user_id]->m_s, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}

void IOCPServer::send_packet_client(int user_id, void* p)
{
	unsigned char* buf = reinterpret_cast<unsigned char*>(p);

	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);

	// IpBuffers 항목에 u의 wsabuf은 이미 Recv에서 쓰고 있기 때문에 사용하면 안됨
	WSASend(g_users[user_id]->m_s, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}

void IOCPServer::process_packet(int user_id, char* buf)
{
	switch (buf[1]) {
	case S2LS_LOGIN:
	{
		std::cout << "The Server " << user_id << " is connected" << endl;
		s2ls_packet_login* packet = reinterpret_cast<s2ls_packet_login*>(buf);
		int sockSize = sizeof(struct sockaddr_in);
		SOCKADDR_IN sock;                                 
		memset(&sock, 0x00, sizeof(SOCKADDR_IN));
		int retval = getpeername(g_servers[user_id]->m_s, (struct sockaddr*) & sock, &sockSize);
		if (retval == 0)
			make_room(user_id, sock);
		else 
			send_login_fail_packet(user_id);
	}
	break;
	case S2LS_LOGOUT:
	{
		std::cout << "The Server " << user_id << " is logout" << endl;
		s2ls_packet_logout* packet = reinterpret_cast<s2ls_packet_logout*>(buf);

		disconnect_server(user_id);
	}
	break;
	case S2LS_HOST_LOGOUT:
	{
		s2ls_packet_host_logout* packet = reinterpret_cast<s2ls_packet_host_logout*>(buf);
		std::cout << "The Room " << user_id << " is Empty" << endl;

		g_rooms[user_id]->make_empty();
	}
	break;
	case C2LS_LOGIN_HOST:
	{
		c2ls_packet_login_host* packet = reinterpret_cast<c2ls_packet_login_host*>(buf);
		g_users[user_id]->o_type = O_HOST;
		int index = -1;
		for (auto& room : g_rooms) {
			if (!room.second->is_host_exist) {
				room.second->terrain_size = packet->terrain_size;
				room.second->frequency = packet->frequency;
				room.second->octaves = packet->octaves;
				room.second->seed = packet->seed;
				room.second->is_host_exist = true;
				strcpy_s(room.second->host_name, packet->name);
				index = room.second->room_id;
				break;
			}
		}

		if (index == -1)
			send_login_fail_host_packet(user_id);
		else {
			g_users[user_id]->m_status = ST_ACTIVE;
			send_login_ok_host_packet(user_id, index);
			for (auto user : g_users) {
				if (user.second->m_status == ST_ACTIVE && user.second->o_type == O_GUEST)
					send_new_room_packet(user.second->m_id, index);
			}
		}
	}
	break; 
	case C2LS_LOGIN_GUEST:
	{
		c2ls_packet_login_guest* packet = reinterpret_cast<c2ls_packet_login_guest*>(buf);
		g_users[user_id]->o_type = O_GUEST;
		g_users[user_id]->m_status = ST_ACTIVE;
		strcpy_s(g_users[user_id]->m_name, packet->name);
		send_login_ok_guest_packet(user_id);

		for (auto room : g_rooms) {
			if (room.second->is_host_exist)
				send_new_room_packet(user_id, room.second->room_id);
		}
	}
	break; 
	default:
		std::cout << "Unknown Packet Type Error!\n" << user_id << ", " << buf << endl;
		//DebugBreak();
		//exit(-1);
		break;
	}
}

void IOCPServer::make_room(int user_id, SOCKADDR_IN& sock) {

	g_servers[user_id]->m_status = ST_ACTIVE;
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(sock.sin_addr), ip, INET_ADDRSTRLEN);
	g_rooms[user_id] = new Room(user_id, ip);
	std::cout << "Room " << user_id << "is created, ip : " << ip << std::endl;

	send_login_ok_packet(user_id);
}

void IOCPServer::delete_room(int user_id)
{
	if (g_rooms[user_id]->is_host_exist){
		// 호스트가 있었던 방이라면 게스트한테 방 없어졌다는 메시지 보내기
		for (auto user : g_users) {
			if (user.second->m_status == ST_ACTIVE && user.second->o_type == O_GUEST)
				send_delete_room_packet(user.second->m_id, user_id);
		}
	}

	delete g_rooms[user_id];
	g_rooms.erase(user_id);

	std::cout << "Room " << user_id << "is deleted" << std::endl;
}

void IOCPServer::disconnect_server(int user_id) {

	std::cout << "Disconnect Server" << user_id << std::endl;

	send_disconnect_packet(user_id);

	g_servers[user_id]->m_status = ST_ALLOC;

	linger sopt_linger = { 0, 0 };
	shutdown(g_servers[user_id]->m_s, SD_BOTH);
	setsockopt(g_servers[user_id]->m_s, SOL_SOCKET, SO_LINGER, (char*)&sopt_linger, sizeof(sopt_linger));
	closesocket(g_servers[user_id]->m_s);
	g_servers[user_id]->m_s = INVALID_SOCKET;

	delete_room(user_id);

	delete g_servers[user_id];
	g_servers.erase(user_id);
}

void IOCPServer::disconnect_client(int user_id)
{
	std::cout << "Disconnect Client" << user_id << std::endl;

	g_users[user_id]->m_status = ST_ALLOC;

	linger sopt_linger = { 0, 0 };
	shutdown(g_users[user_id]->m_s, SD_BOTH);
	setsockopt(g_users[user_id]->m_s, SOL_SOCKET, SO_LINGER, (char*)&sopt_linger, sizeof(sopt_linger));
	closesocket(g_users[user_id]->m_s);
	g_users[user_id]->m_s = INVALID_SOCKET;

	delete g_users[user_id];
	g_users.erase(user_id);
}

void IOCPServer::send_login_ok_packet(int user_id)
{
	ls2s_packet_login_ok p;
	p.size = sizeof(p);
	p.type = LS2S_LOGIN_OK;
	p.server_port = g_rooms[user_id]->port_num;

	send_packet_server(user_id, &p);
}

void IOCPServer::send_login_fail_packet(int user_id)
{
	ls2s_packet_login_fail p;
	p.size = sizeof(p);
	p.type = LS2S_LOGIN_FAIL;

	send_packet_server(user_id, &p);
}

void IOCPServer::send_disconnect_packet(int user_id)
{
	ls2s_packet_disconnect p;
	p.size = sizeof(p);
	p.type = LS2S_DISCONNECT;

	send_packet_server(user_id, &p);
}

void IOCPServer::send_login_ok_host_packet(int user_id, int room_id)
{
	ls2c_packet_login_ok_host p;
	p.size = sizeof(p);
	p.type = LS2C_LOGIN_OK_HOST;
	p.server_port = g_rooms[room_id]->port_num;
	strcpy_s(p.serverIP, g_rooms[room_id]->serverIP);

	send_packet_client(user_id, &p);
}

void IOCPServer::send_login_ok_guest_packet(int user_id)
{
	ls2c_packet_login_ok_guest p;
	p.size = sizeof(p);
	p.type = LS2C_LOGIN_OK_GUEST;

	send_packet_client(user_id, &p);
}

void IOCPServer::send_login_fail_host_packet(int user_id)
{
	ls2c_packet_delete_room p;
	p.size = sizeof(p);
	p.type = LS2C_LOGIN_FAIL;

	send_packet_client(user_id, &p);
}

void IOCPServer::send_new_room_packet(int user_id, int room_id)
{
	ls2c_pakcet_new_room p;
	p.size = sizeof(p);
	p.type = LS2C_NEW_ROOM;
	p.room_id = room_id;
	strcpy_s(p.host_name, g_rooms[room_id]->host_name);
	p.server_port = g_rooms[room_id]->port_num;
	strcpy_s(p.serverIP, g_rooms[room_id]->serverIP);
	p.terrain_size = g_rooms[room_id]->terrain_size;
	p.frequency = g_rooms[room_id]->frequency;
	p.octaves = g_rooms[room_id]->octaves;
	p.seed = g_rooms[room_id]->seed;

	send_packet_client(user_id, &p);
}

void IOCPServer::send_delete_room_packet(int user_id, int room_id)
{
	ls2c_packet_delete_room p;
	p.size = sizeof(p);
	p.type = LS2C_DELETE_ROOM;
	p.room_id = room_id;
	strcpy_s(p.host_name, g_rooms[room_id]->host_name);

	send_packet_client(user_id, &p);
}
