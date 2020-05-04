#include <iostream>
#include <chrono>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "mswsock.lib")
using namespace std;

#include "protocol.h"

constexpr auto MAX_PACKET_SIZE = 255;
constexpr auto MAX_BUF_SIZE = 1024;
//constexpr auto MAX_USER = 10;

enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT };
enum C_STATUS { ST_FREE, ST_ALLOC, ST_ACTIVE };

struct EXOVER {
	WSAOVERLAPPED	over;
	ENUMOP			op;
	char			io_buf[MAX_BUF_SIZE];

	union {	
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
};

struct CLIENT {
	SOCKET	m_s;
	int		m_id;
	EXOVER	m_recv_over;
	int		m_prev_size;
	char	m_packet_buf[MAX_PACKET_SIZE];
	//bool	m_connected;
	C_STATUS	m_status;

	float	x, z;
	float	xMove, zMove;
	char	m_name[MAX_ID_LEN + 1];
};

CLIENT g_clients[MAX_USER];
//int g_curr_user_id = 0;
int g_host_user_id = -1;
HANDLE g_iocp;
SOCKET l_socket;

vector<BuildingInform> buildings;

void send_packet(int user_id, void* p)
{
	char* buf = reinterpret_cast<char*>(p);

	CLIENT& u = g_clients[user_id];

	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);
	// IpBuffers 항목에 u의 wsabuf은 이미 Recv에서 쓰고 있기 때문에 사용하면 안됨
	WSASend(u.m_s, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}

void send_login_ok_packet(int user_id)
{
	sc_packet_login_ok p;
	p.exp = 0;
	p.hp = 0;
	p.id = user_id;
	p.level = 0;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;
	p.x = g_clients[user_id].x;
	p.z = g_clients[user_id].z;
	p.xMove = g_clients[user_id].xMove;
	p.zMove = g_clients[user_id].zMove;

	send_packet(user_id, &p);
}

void send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	p.x = g_clients[o_id].x;
	p.z = g_clients[o_id].z;
	p.xMove = g_clients[user_id].xMove;
	p.zMove = g_clients[user_id].zMove;

	strcpy_s(p.name, g_clients[o_id].m_name);
	if (o_id == g_host_user_id)
		p.o_type = O_HOST;
	else
		p.o_type = O_GUEST;

	//printf("Send_Packet_Enter\n");
	send_packet(user_id, &p);
}

void send_leave_packet(int user_id, int o_id)
{
	sc_packet_leave p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;

	send_packet(user_id, &p);
}

void send_move_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOVE;
	p.x = g_clients[mover].x;
	p.z = g_clients[mover].z;
	p.xMove = g_clients[mover].xMove;
	p.zMove = g_clients[mover].zMove;

	send_packet(user_id, &p);
}

void send_construct_packet(int user_id, BuildingInform b_inform)
{
	sc_packet_construct p;
	p.id = g_host_user_id;
	p.size = sizeof(p);
	p.type = S2C_CONSTRUCT;
	p.b_inform = b_inform;

	printf("전송! buildingType : %d, angle: %f \n", b_inform.buildingType, b_inform.rotAngle);

	send_packet(user_id, &p);
}

void do_move(int user_id, float xMove, float zMove)// int direction)
{
	CLIENT& u = g_clients[user_id];
	float x = u.x;
	float z = u.z;

	// 이 검사식 대신 Normalize 필요
	if (xMove > 4.0f)	xMove = 4.0f;
	else if (xMove < -4.0f)	xMove = -4.0f;
	if (zMove > 4.0f)	zMove = 4.0f;
	else if (zMove < -4.0f)	zMove = -4.0f;

	x += xMove *0.01666;
	z += zMove *0.01666;
	u.x = x;
	u.z = z;
	u.xMove = xMove;
	u.zMove = zMove;

	for (auto& cl : g_clients)
		if (ST_ACTIVE == cl.m_status)
			send_move_packet(cl.m_id, user_id);
}

void enter_game(int user_id, char name[])
{
	strcpy_s(g_clients[user_id].m_name, name);
	g_clients[user_id].m_name[MAX_ID_LEN] = NULL;
	send_login_ok_packet(user_id);

	for (int i = 0; i < MAX_USER; i++)
	{
		if (user_id == i)
			continue;
		if (ST_ACTIVE == g_clients[i].m_status)
			if (user_id != i) {
				send_enter_packet(user_id, i);
				send_enter_packet(i, user_id);
			}
	}
	for (auto& b : buildings)
		send_construct_packet(user_id, b);

	g_clients[user_id].m_status = ST_ACTIVE;
}

void do_construct(int user_id, BuildingInform b_inform)
{
	// 건물 정보를 따로 관리할 자료구조를 생각해보자...
	// 일단은 건물 종류, 위치, 각도만 보내주는 메신저 형식
	buildings.emplace_back(b_inform);
	for (auto& cl : g_clients)
	{
		if (user_id == cl.m_id)
			continue;
		if (ST_ACTIVE == cl.m_status)
			send_construct_packet(cl.m_id, b_inform);
	}
}

void initialize_clients()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		g_clients[i].m_id = i;
		g_clients[i].m_status = ST_FREE;
	}
}

void disconnect(int user_id)
{
	g_clients[user_id].m_status = ST_ALLOC;
	send_leave_packet(user_id, user_id);
	closesocket(g_clients[user_id].m_s);
	if (g_host_user_id != user_id)
	{
		for (auto& cl : g_clients)
		{
			if (user_id == cl.m_id)
				continue;
			if (ST_ACTIVE == cl.m_status)
				send_leave_packet(cl.m_id, user_id);
		}
	}
	else
	{
		for (auto& cl : g_clients)
		{
			if (user_id == cl.m_id)
				continue;
			if (ST_ACTIVE == cl.m_status)
				disconnect(cl.m_id);
		}
		g_host_user_id = -1;
	}
	g_clients[user_id].m_status = ST_FREE;
}

void process_packet(int user_id, char* buf)
{
	switch (buf[1]) {
	case C2S_LOGIN:
	{
		if (g_host_user_id != -1)
		{
			printf("게스트가 놀러왔어요!\n");
			cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);
			enter_game(user_id, packet->name);
		}
		else
		{
			printf("호스트가 없어요!\n");
			disconnect(user_id);
		}
	}
	break;
	case C2S_LOGIN_HOST:
	{
		if (g_host_user_id == -1)
		{
			printf("호스트가 접속했어요!\n");
			cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);
			g_host_user_id = user_id;
			enter_game(user_id, packet->name);
		}
		else
		{
			printf("이미 호스트가 있어요!\n");
			disconnect(user_id);
		}
	}
	break;
	case C2S_MOVE:
	{
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
		do_move(user_id, packet->xMove, packet->zMove);
	}
	break;
	case C2S_CONSTRUCT:
	{
		cs_packet_construct* packet = reinterpret_cast<cs_packet_construct*>(buf);
		if (user_id == g_host_user_id)
			do_construct(user_id, packet->b_inform);
	}
	break;
	default:
		cout << "Unknown Packet Type Error!\n";
		DebugBreak();
		exit(-1);
		break;
	}
}

// 패킷 재조립
void recv_packet_construct(int user_id, int io_byte)
{
	CLIENT& cu = g_clients[user_id];
	EXOVER& r_o = cu.m_recv_over;

	// 나머지 바이트
	int rest_byte = io_byte;
	// 처리할 데이터의 포인터
	char* p = r_o.io_buf;
	int packet_size = 0;
	// 미리 받아논 게 있다면?
	if (0 != cu.m_prev_size)	packet_size = cu.m_packet_buf[0];
	// 처리할 데이터가 남아있다면?
	while (rest_byte > 0)
	{
		// 우리가 처리해야 되는 패킷을 전에 처리해본 적이 없을 때
		// == 패킷의 시작 부분이 있다
		if (0 == packet_size)	packet_size = *p;
		// 패킷을 완성할 수 있다
		if (packet_size <= rest_byte + cu.m_prev_size) {
			memcpy(cu.m_packet_buf + cu.m_prev_size, p, packet_size - cu.m_prev_size);
			p += packet_size - cu.m_prev_size;
			rest_byte -= packet_size - cu.m_prev_size;
			packet_size = 0;	// 이 패킷은 처리되었다
			process_packet(user_id, cu.m_packet_buf);
			cu.m_prev_size = 0;
		}
		else // 완성할 수 없다
		{
			// 저장하여 다음 번에 재조립 해줘야 함
			memcpy(cu.m_packet_buf + cu.m_prev_size, p, rest_byte);
			cu.m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void loop()
{
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);
		CLIENT& cl = g_clients[user_id];

		switch (exover->op) {
		case OP_RECV:
			// send나 recv의 경우에만 이 처리를 해줘야 함
			if (0 == io_byte)
				disconnect(user_id);
			else {
				recv_packet_construct(user_id, io_byte);
				ZeroMemory(&cl.m_recv_over.over, sizeof(cl.m_recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.m_s, &cl.m_recv_over.wsabuf, 1, NULL, &flags, &cl.m_recv_over.over, NULL);
			}
			break;
		case OP_SEND:
			// send나 recv의 경우에만 이 처리를 해줘야 함
			if (0 == io_byte)
				disconnect(user_id);
			delete exover;
			break;
		case OP_ACCEPT:
		{
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i)	{
				if (ST_FREE == g_clients[i].m_status)	{
					g_clients[i].m_status = ST_ALLOC;
					user_id = i;
					break;
				}
			}

			SOCKET c_socket = exover->c_socket;
			if (-1 == user_id)
				closesocket(c_socket);
			else {
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), g_iocp, user_id, 0);

				CLIENT& nc = g_clients[user_id];
				// 안쓰이는 거 골라내는 것도 해야하지만 나중에 생각
				nc.m_prev_size = 0;
				nc.m_recv_over.op = OP_RECV;
				ZeroMemory(&nc.m_recv_over.over, sizeof(nc.m_recv_over.over));
				nc.m_recv_over.wsabuf.buf = nc.m_recv_over.io_buf;
				nc.m_recv_over.wsabuf.len = MAX_BUF_SIZE;
				nc.m_s = c_socket;
				nc.x = 0.0;
				nc.z = 0.0;
				nc.xMove = 0.0;
				nc.zMove = 0.0;
				DWORD flags = 0;
				WSARecv(c_socket, &nc.m_recv_over.wsabuf, 1, NULL, &flags, &nc.m_recv_over.over, NULL);
			}
			c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_socket = c_socket;
			ZeroMemory(&exover->over, sizeof(exover->over));
			AcceptEx(l_socket, c_socket, exover->io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		}
		break;
		}
	}
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));

	listen(l_socket, SOMAXCONN);

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	initialize_clients();

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(l_socket), g_iocp, 999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over;
	ZeroMemory(&accept_over.over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_socket = c_socket;
	AcceptEx(l_socket, c_socket, accept_over.io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);	// 클라이언트 접속에 사용할 소켓을 미리 만들어놔야 함

	loop();
}