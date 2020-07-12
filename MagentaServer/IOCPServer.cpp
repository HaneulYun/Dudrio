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

	l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	// bind -------------------------------------------------------
	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));

	// listen -----------------------------------------------------
	listen(l_socket, SOMAXCONN);
	
	// 네이글 알고리즘 OFF
	//int option = TRUE;
	//setsockopt(l_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));
}

void IOCPServer::start_server()
{
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);// *2 + 1);

	create_worker_threads();
	create_accept_threads();

	cout << "Start_Server Complete" << endl;
}

// thread ---------------------------------
void IOCPServer::create_worker_threads()
{
	//worker_threads.reserve(NUM_OF_CPU * 2 + 2);
	worker_threads.emplace_back([this]() { worker_thread_loop(); });
	//for (int i = 0; i < NUM_OF_CPU * 2 + 1; ++i){
	//	worker_threads.emplace_back([this]() {worker_thread_loop(); });
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
	closesocket(l_socket);

	if (accept_thread.joinable())
		accept_thread.join();

	contents.stop_contents();
	timer.stop_timer();
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

		switch (exover->op) {
		case OP_RECV:
			// send나 recv의 경우에만 이 처리를 해줘야 함
			if (0 == io_byte)
				contents.disconnect(user_id);
			else {
				recv_packet_construct(user_id, io_byte);
				ZeroMemory(&g_clients[user_id]->m_recv_over.over, sizeof(g_clients[user_id]->m_recv_over.over));
				DWORD flags = 0;
				WSARecv(g_clients[user_id]->m_s, &g_clients[user_id]->m_recv_over.wsabuf, 1, NULL, &flags, &g_clients[user_id]->m_recv_over.over, NULL);
			}
			break;
		case OP_SEND:
			// send나 recv의 경우에만 이 처리를 해줘야 함
			if (0 == io_byte)
				contents.disconnect(user_id);
			delete exover;
			break;
		//case OP_ACCEPT:
		//{
		//	SOCKET c_socket = exover->c_socket;
		//	int idx = 0;
		//	bool flag = false;
		//	for (auto& cl : g_clients){
		//		if (idx != cl.first) {
		//			g_clients[idx] = new Clients(c_socket, idx);
		//			g_clients[idx]->m_status = ST_ALLOC;
		//			flag = true;
		//			break;
		//		}
		//		else
		//			idx++;
		//	}
		//
		//	if (g_clients.empty())
		//	{
		//		g_clients[idx] = new Clients(c_socket, idx);
		//		flag = true;
		//	}
		//	else if (idx == g_clients.size() && !flag)
		//	{
		//		g_clients[idx] = new Clients(c_socket, idx);
		//		flag = true;
		//	}
		//
		//	if (!flag) {
		//		closesocket(c_socket);
		//	}
		//	else {
		//		CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), g_iocp, idx, 0);
		//
		//		DWORD flags = 0;
		//		WSARecv(c_socket, &g_clients[idx]->m_recv_over.wsabuf, 1, NULL, &flags, &g_clients[idx]->m_recv_over.over, NULL);
		//	}
		//	c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		//	exover->c_socket = c_socket;
		//	ZeroMemory(&exover->over, sizeof(exover->over));
		//	AcceptEx(l_socket, c_socket, exover->io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		//}
		//break;
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
		bool flag = false;

		for (auto& cl : g_clients) {
			if (idx != cl.first) {
				g_clients[idx] = new Clients(idx);
				g_clients[idx]->m_status = ST_ALLOC;
				flag = true;
				break;
			}
			else
				idx++;
			if (idx >= MAX_USER)
				break;
		}

		if (idx >= MAX_USER)
			continue;

		if (g_clients.empty()){
			g_clients[idx] = new Clients(idx);
			flag = true;
		}
		else if (idx == g_clients.size() && !flag){
			g_clients[idx] = new Clients(idx);
			flag = true;
		}

		ZeroMemory(&g_clients[idx]->m_recv_over.over, sizeof(g_clients[idx]->m_recv_over.over));
		g_clients[idx]->m_s = WSAAccept(l_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len, NULL, NULL);
		if (INVALID_SOCKET == g_clients[idx]->m_s)	continue;

		bool retval = CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_clients[idx]->m_s), g_iocp, idx, 0);
		if (false == retval) {
			cout << "Bind IO Completion Port Error" << GetLastError() << endl;
			closesocket(g_clients[idx]->m_s);
			delete g_clients[idx];
			g_clients.erase(idx);
			continue;
		}

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);

		// Recv Overlapped I/O작업을 요청해 놓는다.
		DWORD flags = 0;
		g_clients[idx]->m_recv_over.c_socket = g_clients[idx]->m_s;
		g_clients[idx]->m_recv_over.op = OP_RECV;
		retval = WSARecv(g_clients[idx]->m_s, &g_clients[idx]->m_recv_over.wsabuf, 1, NULL, &flags, &g_clients[idx]->m_recv_over.over, NULL);

		if (retval == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
			cout << "Bind Recv Operation Error" << WSAGetLastError() << endl;
			closesocket(g_clients[idx]->m_s);
			delete g_clients[idx];
			g_clients.erase(idx);
			continue;
		}
	}
}

void IOCPServer::recv_packet_construct(int user_id, int io_byte)
{
	EXOVER& r_o = g_clients[user_id]->m_recv_over;

	// 나머지 바이트
	int rest_byte = io_byte;
	// 처리할 데이터의 포인터
	char* p = r_o.io_buf;
	int packet_size = 0;
	// 미리 받아논 게 있다면?
	if (0 != g_clients[user_id]->m_prev_size)	packet_size = g_clients[user_id]->m_packet_buf[0];
	// 처리할 데이터가 남아있다면?
	while (rest_byte > 0)
	{
		// 우리가 처리해야 되는 패킷을 전에 처리해본 적이 없을 때
		// == 패킷의 시작 부분이 있다
		if (0 == packet_size)	packet_size = *p;
		// 패킷을 완성할 수 있다
		if (packet_size <= rest_byte + g_clients[user_id]->m_prev_size) {
			memcpy(g_clients[user_id]->m_packet_buf + g_clients[user_id]->m_prev_size, p, packet_size - g_clients[user_id]->m_prev_size);
			p += packet_size - g_clients[user_id]->m_prev_size;
			rest_byte -= packet_size - g_clients[user_id]->m_prev_size;
			packet_size = 0;	// 이 패킷은 처리되었다
			contents.add_packet(user_id, g_clients[user_id]->m_packet_buf);
			g_clients[user_id]->m_prev_size = 0;
		}
		else // 완성할 수 없다
		{
			// 저장하여 다음 번에 재조립 해줘야 함
			memcpy(g_clients[user_id]->m_packet_buf + g_clients[user_id]->m_prev_size, p, rest_byte);
			g_clients[user_id]->m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void IOCPServer::send_packet(int user_id, void* p)
{
	char* buf = reinterpret_cast<char*>(p);

	//CLIENT& u = g_clients[user_id];

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
	p.x = g_clients[user_id]->xPos;
	p.z = g_clients[user_id]->zPos;
	p.xMove = g_clients[user_id]->xVel;
	p.zMove = g_clients[user_id]->zVel;
	p.rotAngle = g_clients[user_id]->rotAngle;

	send_packet(user_id, &p);
}

void IOCPServer::send_login_fail_packet(int user_id)
{
	sc_packet_login_fail p;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_FAIL;

	send_packet(user_id, &p);
}

void IOCPServer::send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	p.x = g_clients[o_id]->xPos;
	p.z = g_clients[o_id]->zPos;
	p.xMove = g_clients[o_id]->xVel;
	p.zMove = g_clients[o_id]->zVel;
	p.rotAngle = g_clients[o_id]->rotAngle;

	strcpy_s(p.name, g_clients[o_id]->m_name);
	if (o_id == contents.host_id)
		p.o_type = O_HOST;
	else
		p.o_type = O_GUEST;

	send_packet(user_id, &p);
}

void IOCPServer::send_leave_packet(int user_id, int o_id)
{
	sc_packet_leave p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;

	send_packet(user_id, &p);
}

//void IOCPServer::send_move_packet(int user_id, int mover)
//{
//
//}

void IOCPServer::send_move_start_packet(int user_id, int mover)
{
	sc_packet_move_start p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOVE_START;
	p.x = g_clients[mover]->xPos;
	p.z = g_clients[mover]->zPos;
	p.xMove = g_clients[mover]->xVel;
	p.zMove = g_clients[mover]->zVel;

	send_packet(user_id, &p);
}

void IOCPServer::send_move_end_packet(int user_id, int mover)
{
	sc_packet_move_end p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOVE_END;
	p.x = g_clients[mover]->xPos;
	p.z = g_clients[mover]->zPos;

	send_packet(user_id, &p);
}

void IOCPServer::send_rotate_packet(int user_id, int rotater, float dAngle)
{
	sc_packet_rotate p;
	p.id = rotater;
	p.size = sizeof(p);
	p.type = S2C_ROTATE;
	p.rotAngle = dAngle;
	p.x = g_clients[rotater]->xPos;
	p.z = g_clients[rotater]->zPos;
	p.xMove = g_clients[rotater]->xVel;
	p.zMove = g_clients[rotater]->zVel;

	printf("x: %f, z: %f, xVel: %f, zVel: %f, rotAngle: %f\n", p.x, p.z, p.xMove, p.zMove, p.rotAngle);
	send_packet(user_id, &p);
}

void IOCPServer::send_construct_packet(int user_id, BuildingInform b_inform)
{
	sc_packet_construct p;
	p.id = 0;
	p.size = sizeof(p);
	p.type = S2C_CONSTRUCT;
	p.b_inform = b_inform;

	send_packet(user_id, &p);
}

void IOCPServer::send_destruct_packet(int user_id, BuildingInform b_inform)
{
	sc_packet_destruct p;
	p.id = 0;
	p.size = sizeof(p);
	p.type = S2C_DESTRUCT;
	p.b_inform = b_inform;

	send_packet(user_id, &p);
}

void IOCPServer::send_destruct_all_packet(int user_id)
{
	sc_packet_destruct_all p;
	p.id = 0;
	p.size = sizeof(p);
	p.type = S2C_DESTRUCT_ALL;

	send_packet(user_id, &p);
}

void IOCPServer::send_chat_packet(int user_id, int chatter, wchar_t mess[])
{
	sc_packet_chat p;
	p.id = chatter;
	p.size = sizeof(p);
	p.type = S2C_CHAT;
	wcscpy_s(p.mess, mess);

	send_packet(user_id, &p);
}