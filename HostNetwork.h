#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <unordered_map>

#define BUFSIZE 200

using namespace std;

class HostNetwork : public MonoBehavior<HostNetwork>
{
private:

public:
	WSADATA WSAData;
	std::wstring wserverIp;
	SOCKET serverSocket;
	int myId;
	char name[MAX_ID_LEN];
	bool isConnect{ false };

	static HostNetwork* network;

public:
	GameObject* simsPrefab = NULL;
	unordered_map<int, GameObject*> players;

private:
	friend class GameObject;
	friend class MonoBehavior<HostNetwork>;
	HostNetwork() = default;
	HostNetwork(HostNetwork&) = default;

public:
	~HostNetwork() {
		closesocket(serverSocket);
		WSACleanup();
	}

	void ProcessPacket(char* ptr);
	void process_data(char* net_buf, size_t io_byte);

	void Receiver();

	void send_packet(void* packet);
	void send_construct_packet(BuildingInform b_inform);

	void Login();

	void Start()
	{
	
	}

	void Update()
	{
		if (isConnect)
		{
			Receiver();
		}
	}

	void PressButton()
	{
		if (!isConnect)
		{
			WSAStartup(MAKEWORD(2, 0), &WSAData);
			serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
			unsigned long on = true;
			int nRet = ioctlsocket(serverSocket, FIONBIO, &on);

			std::wstring title(L"Input IP Here : ");
			SetWindowText(CyanApp::GetHwnd(), title.c_str());

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
	}
};