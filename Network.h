#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"
#include "CharacterController.h"
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <unordered_map>

#define BUFSIZE 200

using namespace std;

class Network : public MonoBehavior<Network>
{
private:

public:
	WSADATA WSAData;
	std::wstring wserverIp;
	SOCKET serverSocket;
	int myId;
	bool isConnect{ false };

public:
	GameObject* myCharacter = NULL;

	//Pawn* pawns[MAX_USER - 1];
	//GameObject* myPawn;
	//Pawn* myPawnPtr = NULL;
	//unordered_map<int, Pawn*> otherPawns;

private:
	friend class GameObject;
	friend class MonoBehavior<Network>;
	Network() = default;
	Network(Network&) = default;

public:
	~Network() {
		closesocket(serverSocket);
		WSACleanup();
	}

	//Pawn* GetDisconnectedPawn()
	//{
	//	for (int i = 0; i < MAX_USER - 1; ++i)
	//	{
	//		if (!pawns[i]->show)
	//			return pawns[i];
	//	}
	//	return nullptr;
	//}

	void ProcessPacket(char* ptr)
	{
		static bool first_time = true;
		switch (ptr[1])
		{
		case S2C_LOGIN_OK:
		{
			sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
			myId = my_packet->id;
			myCharacter->GetComponent<CharacterController>()->move(my_packet->x, my_packet->z);
		}
		break;

		case S2C_ENTER:
		{
			sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
			int id = my_packet->id;

			if (id == myId) {
				myCharacter->GetComponent<CharacterController>()->move(my_packet->x, my_packet->z);
			}
			//else {
			//	otherPawns[id] = GetDisconnectedPawn();
			//	if (otherPawns[id] != nullptr)
			//	{
			//		otherPawns[id]->show = true;
			//		//strcpy_s(otherPawnPtr->name, my_packet->name);
			//		strcpy_s(otherPawns[id]->name, my_packet->name);
			//		//otherPawnPtr->move(my_packet->x, my_packet->y);
			//		otherPawns[id]->move(my_packet->x, my_packet->y);
			//	}
			//}
		}
		break;
		case S2C_MOVE:
		{
			sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
			int other_id = my_packet->id;
			if (other_id == myId) {
				myCharacter->GetComponent<CharacterController>()->move(my_packet->x, my_packet->z);
			}
			//else {
			//	if (0 != otherPawns.count(other_id))
			//		otherPawns[other_id]->move(my_packet->x, my_packet->y);
			//}
		}
		break;

		case S2C_LEAVE:
		{
			sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
			int other_id = my_packet->id;
			if (other_id == myId) {
				DeleteObject(myCharacter);
			}
			//else {
			//	if (0 != otherPawns.count(other_id))
			//	{
			//		otherPawns[other_id]->hide();
			//		otherPawns.erase(other_id);
			//	}
			//}
		}
		break;
		default:
			printf("Unknown PACKET type [%d]\n", ptr[1]);
		}
	}

	void process_data(char* net_buf, size_t io_byte)
	{
		char* ptr = net_buf;
		static size_t in_packet_size = 0;
		static size_t saved_packet_size = 0;
		static char packet_buffer[BUFSIZE];

		while (0 != io_byte) {
			if (0 == in_packet_size) in_packet_size = ptr[0];
			if (io_byte + saved_packet_size >= in_packet_size) {
				memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
				ProcessPacket(packet_buffer);
				ptr += in_packet_size - saved_packet_size;
				io_byte -= in_packet_size - saved_packet_size;
				in_packet_size = 0;
				saved_packet_size = 0;
			}
			else {
				memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
				saved_packet_size += io_byte;
				io_byte = 0;
			}
		}
	}

	void Receiver()
	{
		char net_buf[BUFSIZE];
		auto retval = recv(serverSocket, net_buf, BUFSIZE, 0);

		if (retval > 0)	process_data(net_buf, retval);
	}

	void send_packet(void* packet)
	{
		char* p = reinterpret_cast<char*>(packet);
		send(serverSocket, p, p[0], 0);
	}

	void send_move_packet(unsigned char dir)
	{
		cs_packet_move m_packet;
		m_packet.type = C2S_MOVE;
		m_packet.size = sizeof(m_packet);
		m_packet.direction = dir;
		send_packet(&m_packet);
	}

	void Login()
	{
		cs_packet_login l_packet;
		l_packet.size = sizeof(l_packet);
		l_packet.type = C2S_LOGIN;
		int t_id = GetCurrentProcessId();
		sprintf_s(l_packet.name, "P%03d", t_id % 1000);
		strcpy_s(myCharacter->GetComponent<CharacterController>()->name, l_packet.name);
		send_packet(&l_packet);
	}

	void Start()
	{
		WSAStartup(MAKEWORD(2, 0), &WSAData);
		serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
		unsigned long on = true;
		int nRet = ioctlsocket(serverSocket, FIONBIO, &on);

		std::wstring title(L"Input IP Here : ");
		SetWindowText(CyanApp::GetHwnd(), title.c_str());
	}

	void Update()
	{
		if (!isConnect)
		{
			std::string serverIp = "127.0.0.1";
			//serverIp.assign(wserverIp.begin(), wserverIp.end());

			SOCKADDR_IN serveraddr{};
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
			serveraddr.sin_port = htons(SERVER_PORT);

			int retval = connect(serverSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

			isConnect = true;
			Login();
		}
		else
		{
			if (Input::GetKeyDown(KeyCode::W))
			{
				send_move_packet(D_UP);
			}
			else if (Input::GetKeyDown(KeyCode::A))
			{
				send_move_packet(D_LEFT);
			}
			else if (Input::GetKeyDown(KeyCode::S))
			{
				send_move_packet(D_DOWN);
			}
			else if (Input::GetKeyDown(KeyCode::D))
			{
				send_move_packet(D_RIGHT);
			}
			Receiver();
		}
	}
};