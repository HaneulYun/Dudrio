#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <unordered_map>

#define BUFSIZE 200

using namespace std;

class GuestNetwork : public MonoBehavior<GuestNetwork>
{
private:

public:
	WSADATA WSAData;
	std::wstring wserverIp;
	SOCKET serverSocket;
	int myId;
	int hostId;
	bool isConnect{ false };

public:
	GameObject* simsPrefab = NULL;
	GameObject* myCharacter = NULL;
	unordered_map<int, GameObject*> otherCharacters;

private:
	friend class GameObject;
	friend class MonoBehavior<GuestNetwork>;
	GuestNetwork() = default;
	GuestNetwork(GuestNetwork&) = default;

public:
	~GuestNetwork() {
		closesocket(serverSocket);
		WSACleanup();
	}

	void ProcessPacket(char* ptr);
	void process_data(char* net_buf, size_t io_byte);

	void Receiver();

	void send_packet(void* packet);
	void send_move_packet(float xMove, float zMove);

	void Login();

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
			Receiver();
		}
	}

};