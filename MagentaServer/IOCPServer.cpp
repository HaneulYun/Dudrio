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

	l_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	// bind -------------------------------------------------------
	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(server_port);
	cout << "Server port num: " << server_port << endl;
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));

	// listen -----------------------------------------------------
	listen(l_socket, SOMAXCONN);

	int option = TRUE;
	setsockopt(l_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));
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
	for (auto& cl : g_clients)
		delete cl.second;
	g_clients.clear();

	g_clients.reserve(MAX_USER);
}

// thread ---------------------------------
void IOCPServer::create_worker_threads()
{
	worker_threads.reserve(NUM_OF_CPU * 2 + 2);
	//worker_threads.emplace_back([this]() { worker_thread_loop(); });
	for (int i = 0; i < NUM_OF_CPU * 2 + 1; ++i) {
		worker_threads.emplace_back([this]() {worker_thread_loop(); });
	}

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

		g_clients_lock.lock();
		if (g_clients.count(user_id) == 0 && (exover->op == OP_RECV || exover->op == OP_SEND))
		{
			cout << "저한테 왜그러세요" << user_id << endl;
			g_clients_lock.unlock();
			continue;
		}
		g_clients_lock.unlock();

		switch (exover->op) {
		case OP_RECV:
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
			if (0 == io_byte)
				contents.disconnect(user_id);
			delete exover;
			break;
		// Game Contents
		case GAME_UPDATE: 
		{
			contents.update();
			delete exover;
		}
			break;
		case GAME_TIME:
		{
			g_clients_lock.lock();
			for (auto cl : g_clients) {
				if (ST_ACTIVE == cl.second->m_status)
					send_game_time_packet(cl.second->m_id);
			}
			g_clients_lock.unlock();
			delete exover;
		}
		break;
		case SIM_BUILD: 
		{
			g_sims_lock.lock();
			if (g_sims.count(exover->target_id) != 0) {
				g_sims_lock.unlock();
				g_sims[exover->target_id]->buildInfo = *static_cast<BuildMessageInfo*>(exover->extra_info);
				SIM_Message msg{ SIM_Build, exover->extra_info };
				g_sims[exover->target_id]->stateMachine.HandleMessage(msg);
			}
			else
				g_sims_lock.unlock();
			delete exover;
		}
			break;
		case SIM_MOVE:
		{
			g_sims_lock.lock();
			if (g_sims.count(exover->target_id) != 0) {
				g_sims_lock.unlock();
				SIM_Message msg{ SIM_Move, NULL };
				g_sims[exover->target_id]->stateMachine.HandleMessage(msg);
			}
			else
				g_sims_lock.unlock();
			delete exover;
		}
			break;
		case SIM_SLEEP:
		{
			g_sims_lock.lock();
			if (g_sims.count(exover->target_id) != 0) {
				g_sims_lock.unlock();
				SIM_Message msg{ SIM_Sleep, NULL };
				g_sims[exover->target_id]->stateMachine.HandleMessage(msg);
			}
			else
				g_sims_lock.unlock();
			delete exover;
		}
			break;
		case SIM_WAKEUP:
		{
			g_sims_lock.lock();
			if (g_sims.count(exover->target_id) != 0) {
				g_sims_lock.unlock();
				SIM_Message msg{ SIM_WakeUp, NULL };
				g_sims[exover->target_id]->stateMachine.HandleMessage(msg);
			}
			else
				g_sims_lock.unlock();
			delete exover;
		}
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

		g_clients_lock.lock();
		while (idx < MAX_USER) {
			if (g_clients.count(idx) == 0) {
				cout << "New idx " << idx << " is generated" << endl;
				g_clients[idx] = new Client(idx);
				g_clients[idx]->m_status = ST_ALLOC;
				break;
			}
			else {
				idx++;
			}
		}
		g_clients_lock.unlock();

		if (idx >= MAX_USER)
			continue;

		lock_guard<mutex>lock_guard_client(g_clients[idx]->m_cl);
		ZeroMemory(&g_clients[idx]->m_recv_over.over, sizeof(g_clients[idx]->m_recv_over.over));
		g_clients[idx]->m_s = WSAAccept(l_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len, NULL, NULL);
		if (INVALID_SOCKET == g_clients[idx]->m_s)	continue;

		bool retval = CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_clients[idx]->m_s), g_iocp, idx, 0);
		if (false == retval) {
			lock_guard<mutex>lock_guard(g_clients_lock);
			cout << "Bind IO Completion Port Error" << GetLastError() << endl;
			closesocket(g_clients[idx]->m_s);
			delete g_clients[idx];
			g_clients.erase(idx);
			continue;
		}

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);

		DWORD flags = 0;
		g_clients[idx]->m_recv_over.c_socket = g_clients[idx]->m_s;
		g_clients[idx]->m_recv_over.op = OP_RECV;
		retval = WSARecv(g_clients[idx]->m_s, &g_clients[idx]->m_recv_over.wsabuf, 1, NULL, &flags, &g_clients[idx]->m_recv_over.over, NULL);

		if (retval == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
			lock_guard<mutex>lock_guard(g_clients_lock);
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
	if (0 != g_clients[user_id]->m_prev_size)	packet_size = (unsigned char)g_clients[user_id]->m_packet_buf[0];
	// 처리할 데이터가 남아있다면?
	while (rest_byte > 0)
	{
		// 우리가 처리해야 되는 패킷을 전에 처리해본 적이 없을 때
		// == 패킷의 시작 부분이 있다
		if (0 == packet_size)	packet_size = (unsigned char)*p;
		// 패킷을 완성할 수 있다
		if (packet_size <= rest_byte + g_clients[user_id]->m_prev_size) {
			memcpy(g_clients[user_id]->m_packet_buf + g_clients[user_id]->m_prev_size, p, packet_size - g_clients[user_id]->m_prev_size);
			p += packet_size - g_clients[user_id]->m_prev_size;
			rest_byte -= packet_size - g_clients[user_id]->m_prev_size;
			packet_size = 0;	// 이 패킷은 처리되었다
			contents.process_packet(user_id, g_clients[user_id]->m_packet_buf);
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
	wcscpy(p.host_name, g_clients[contents.host_id]->m_name);

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

void IOCPServer::send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	p.xPos = g_clients[o_id]->m_xPos;
	p.zPos = g_clients[o_id]->m_zPos;
	p.xVel = g_clients[o_id]->m_xVel;
	p.zVel = g_clients[o_id]->m_zVel;
	p.rotAngle = g_clients[o_id]->m_rotAngle;

	wcscpy_s(p.name, g_clients[o_id]->m_name);
	if (o_id == contents.host_id)
		p.o_type = O_HOST;
	else
		p.o_type = O_GUEST;

	if (user_id != contents.host_id && o_id != contents.host_id
		&& user_id != o_id) {
		g_clients[user_id]->m_cl.lock();
		g_clients[user_id]->view_list.insert(o_id);
		g_clients[user_id]->m_cl.unlock();
	}

	send_packet(user_id, &p);
}

void IOCPServer::send_leave_packet(int user_id, int o_id)
{
	sc_packet_leave p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;

	if (user_id != contents.host_id && o_id != contents.host_id
		&& user_id != o_id) {
		g_clients[user_id]->m_cl.lock();
		g_clients[user_id]->view_list.erase(o_id);
		g_clients[user_id]->m_cl.unlock();
	}

	send_packet(user_id, &p);
}

void IOCPServer::send_move_packet(int user_id, int mover, float dAngle)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOVE;
	p.xPos = g_clients[mover]->m_xPos;
	p.zPos = g_clients[mover]->m_zPos;
	p.xVel = g_clients[mover]->m_xVel;
	p.zVel = g_clients[mover]->m_zVel;
	p.rotAngle = dAngle;
	p.move_time = g_clients[mover]->m_move_time;

	send_packet(user_id, &p);
}

void IOCPServer::send_enter_sim_packet(int user_id, int o_id)
{
	sc_packet_sim_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_SIM_ENTER;
	p.xPos = g_sims[o_id]->pos.x;
	p.zPos = g_sims[o_id]->pos.z;
	p.xVel = g_sims[o_id]->forward.x;
	p.zVel = g_sims[o_id]->forward.z;
	p.rotAngle = g_sims[o_id]->rotAngle;
	
	g_clients[user_id]->m_cl.lock();
	g_clients[user_id]->sim_list.insert(o_id);
	g_clients[user_id]->m_cl.unlock();

	send_packet(user_id, &p);
}

void IOCPServer::send_leave_sim_packet(int user_id, int o_id)
{
	sc_packet_sim_leave p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_SIM_LEAVE;

	g_clients[user_id]->m_cl.lock();
	g_clients[user_id]->sim_list.erase(o_id);
	g_clients[user_id]->m_cl.unlock();

	send_packet(user_id, &p);
}

void IOCPServer::send_move_sim_packet(int user_id, int mover, float dAngle)
{
	sc_packet_sim_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_SIM_MOVE;
	p.xPos = g_sims[mover]->pos.x;
	p.zPos = g_sims[mover]->pos.z;
	p.xVel = g_sims[mover]->forward.x;
	p.zVel = g_sims[mover]->forward.z;
	p.rotAngle = dAngle;

	send_packet(user_id, &p);
}

void IOCPServer::send_construct_packet(int user_id, int type, int name, float x, float z, float angle, int range)
{
	sc_packet_construct p;
	p.size = sizeof(p);
	p.type = S2C_CONSTRUCT;
	p.landmark_range = range;
	p.building_name = name;
	p.building_type = type;
	p.xPos = x;
	p.zPos = z;
	p.angle = angle;

	send_packet(user_id, &p);
}

void IOCPServer::send_destruct_packet(int user_id, int type, int name, float x, float z)
{
	sc_packet_destruct p;
	p.size = sizeof(p);
	p.type = S2C_DESTRUCT;
	p.building_name = name;
	p.building_type = type;
	p.xPos = x;
	p.zPos = z;

	send_packet(user_id, &p);
}

void IOCPServer::send_destruct_all_packet(int user_id)
{
	sc_packet_destruct_all p;
	p.size = sizeof(p);
	p.type = S2C_DESTRUCT_ALL;

	send_packet(user_id, &p);
}

void IOCPServer::send_game_time_packet(int user_id)
{
	sc_packet_game_time p;
	p.size = sizeof(p);
	p.type = S2C_GAME_TIME;
	p.game_time = contents.ingame_time;

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