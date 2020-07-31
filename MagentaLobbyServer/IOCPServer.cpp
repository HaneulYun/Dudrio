#include "pch.h"
#include "IOCPServer.h"

IOCPServer::IOCPServer()
{
	worker_run = true;
	accept_run = true;
}

IOCPServer::~IOCPServer()
{
	WSACleanup();
	destroy_threads();
}

void IOCPServer::init_server()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	sl_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	// bind -------------------------------------------------------
	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_TO_LOBBY_SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(sl_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));

	// listen -----------------------------------------------------
	listen(sl_socket, SOMAXCONN);

	int option = TRUE;
	setsockopt(sl_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));
}

void IOCPServer::start_server()
{
	init_clients();

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NUM_OF_CPU * 2 + 1);

	create_worker_threads();
	create_accept_threads();

	cout << "Start_Server Complete" << endl;
}

void IOCPServer::init_clients()
{
	for (auto& cl : g_servers)
		delete cl.second;
	g_servers.clear();

	g_servers.reserve(MAX_USER);
}

// thread ---------------------------------
void IOCPServer::create_worker_threads()
{
	//worker_threads.reserve(NUM_OF_CPU * 2 + 2);
	//
	//for (int i = 0; i < NUM_OF_CPU * 2 + 1; ++i) {
		worker_threads.emplace_back([this]() {worker_thread_loop(); });
	//}

	cout << "Create Worker_Threads Complete" << endl;
}

void IOCPServer::create_accept_threads()
{
	accept_thread = thread([this]() { accept_thread_loop(); });

	cout << "Create Accept_Thread Complete" << endl;
}

void IOCPServer::destroy_threads()
{
	worker_run = false;
	CloseHandle(g_iocp);
	for (auto& w_t : worker_threads)
		if (w_t.joinable())
			w_t.join();

	accept_run = false;
	closesocket(sl_socket);

	if (accept_thread.joinable())
		accept_thread.join();
}

void IOCPServer::worker_thread_loop()
{
	while (worker_run) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);

		//g_clients_lock.lock();
		//if (g_clients.count(user_id) == 0 && (exover->op == OP_RECV || exover->op == OP_SEND))
		//{
		//	cout << "저한테 왜그러세요" << user_id << endl;
		//	g_clients_lock.unlock();
		//	continue;
		//}
		//g_clients_lock.unlock();

		switch (exover->op) {
		case OP_RECV:
			if (0 == io_byte)
				disconnect_server(user_id);
			else {
				recv_packet_construct(user_id, io_byte);
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
			cout << "Invalid Operation " << exover->op << endl;
			break;
		}
	}
}

void IOCPServer::accept_thread_loop()
{
	SOCKADDR_IN client_addr;
	auto client_len = static_cast<int>(sizeof(client_addr));

	while (accept_run)
	{
		// 접속 받을 유저 소켓을 생성 한다.
		int idx = 0;

		//g_clients_lock.lock();
		while (idx < MAX_SERVER) {
			if (g_servers.count(idx) == 0) {
				cout << "New idx " << idx << " is generated" << endl;
				g_servers[idx] = new Client(idx);
				g_servers[idx]->m_status = ST_ALLOC;
				break;
			}
			else {
				idx++;
			}
		}
		//g_clients_lock.unlock();

		if (idx >= MAX_USER)
			continue;

		//lock_guard<mutex>lock_guard_client(g_clients[idx]->m_cl);
		ZeroMemory(&g_servers[idx]->m_recv_over.over, sizeof(g_servers[idx]->m_recv_over.over));
		g_servers[idx]->m_s = WSAAccept(sl_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len, NULL, NULL);
		if (INVALID_SOCKET == g_servers[idx]->m_s)	
			continue;

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

void IOCPServer::disconnect_client(int user_id)
{

}

void IOCPServer::recv_packet_construct(int user_id, int io_byte)
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

void IOCPServer::process_packet(int user_id, char* buf)
{

}

void IOCPServer::disconnect_server(int user_id) {
	cout << "Disconnect " << user_id << endl;

	iocp.send_leave_packet(user_id, user_id);

	if (host_id != user_id)
		g_clients[user_id]->erase_client_in_sector();
	g_clients[user_id]->m_cl.lock();
	g_clients[user_id]->m_status = ST_ALLOC;

	linger sopt_linger = { 0, 0 };
	shutdown(g_clients[user_id]->m_s, SD_BOTH);
	setsockopt(g_clients[user_id]->m_s, SOL_SOCKET, SO_LINGER, (char*)&sopt_linger, sizeof(sopt_linger));
	closesocket(g_clients[user_id]->m_s);
	g_clients[user_id]->m_s = INVALID_SOCKET;

	if (host_id != user_id) {
		for (auto cl = g_clients[user_id]->view_list.begin(); cl != g_clients[user_id]->view_list.end(); ++cl) {
			if (user_id == *cl) continue;
			if (ST_ACTIVE == g_clients[*cl]->m_status)
				iocp.send_leave_packet(*cl, user_id);
		}
		if (host_id != -1)
			iocp.send_leave_packet(host_id, user_id);
	}
	else {
		for (auto cl = g_clients.begin(); cl != g_clients.end();) {
			if (cl->first == user_id) { ++cl; continue; }
			if (ST_ACTIVE == cl->second->m_status) {
				cout << "Disconnect " << cl->second->m_id << endl;
				iocp.send_leave_packet(cl->first, cl->first);

				cl->second->m_cl.lock();
				cl->second->m_status = ST_ALLOC;

				linger sopt_linger = { 0, 0 };
				shutdown(g_clients[user_id]->m_s, SD_BOTH);
				setsockopt(g_clients[user_id]->m_s, SOL_SOCKET, SO_LINGER, (char*)&sopt_linger, sizeof(sopt_linger));
				closesocket(g_clients[user_id]->m_s);
				g_clients[user_id]->m_s = INVALID_SOCKET;

				cl->second->m_cl.unlock();
				delete cl->second;
				cl = g_clients.erase(cl);
			}
			else
				++cl;
		}

		init_contents();

		cout << "Disconnect the host" << endl;
	}
	g_clients[user_id]->m_cl.unlock();

	delete g_clients[user_id];
	g_clients.erase(user_id);
}

void IOCPServer::send_packet(int user_id, void* p)
{
	unsigned char* buf = reinterpret_cast<unsigned char*>(p);

	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);

	// IpBuffers 항목에 u의 wsabuf은 이미 Recv에서 쓰고 있기 때문에 사용하면 안됨
	WSASend(g_clients[user_id]->m_s, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}

void IOCPServer::send_login_ok_packet(int user_id)
{
	sc_packet_login_ok p;
	p.id = user_id;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;
	p.xPos = g_clients[user_id]->m_xPos;
	p.zPos = g_clients[user_id]->m_zPos;
	p.xVel = g_clients[user_id]->m_xVel;
	p.zVel = g_clients[user_id]->m_zVel;
	p.rotAngle = g_clients[user_id]->m_rotAngle;
	p.game_time = contents.ingame_time;
	p.host_id = contents.host_id;
	strcpy(p.host_name, g_clients[contents.host_id]->m_name);

	if (terrain_data != nullptr) {
		p.frequency = terrain_data->frequency;
		p.terrainSize = terrain_data->terrain_size;
		p.octaves = terrain_data->octaves;
		p.seed = terrain_data->seed;
	}

	cout << "Ingame time: " << p.game_time << endl;
	send_packet(user_id, &p);
}

void IOCPServer::send_login_fail_packet(int user_id)
{
	sc_packet_login_fail p;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_FAIL;

	send_packet(user_id, &p);
}