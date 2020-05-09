#pragma once
#include "CyanEngine\CyanEngine\framework.h"
//#include "MagentaServer\protocol.h"
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
	bool tryConnect{ false };
	int retval;
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
	void send_destruct_packet(BuildingInform b_inform);
	void send_destruct_all_packet();

	void Login();

	void Start()
	{
	
	}

	void Update()
	{
		if (tryConnect)
		{
			if (retval == SOCKET_ERROR)
				tryConnect = false;
			else if (retval == 0)
			{
				tryConnect = false;
				isConnect = true;
				unsigned long on = true;
				int nRet = ioctlsocket(serverSocket, FIONBIO, &on);
				Login();
			}
		}
		if (isConnect)
		{
			Receiver();
		}
	}

	int connect_nonblock(SOCKET sockfd, const struct sockaddr FAR* name, int namelen, int timeout)
	{
		unsigned long nonzero = 1;
		unsigned long zero = 0;
		fd_set rset, wset;
		struct timeval tval;
		int n;
		int nfds = 1;

		if (ioctlsocket(sockfd, FIONBIO, &nonzero) == SOCKET_ERROR)
			return SOCKET_ERROR;

		if ((n = connect(sockfd, (struct sockaddr FAR*)name, namelen)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				return SOCKET_ERROR;
		}

		if (n == 0)
			goto done;

		FD_ZERO(&rset);
		FD_SET(sockfd, &rset);
		wset = rset;
		tval.tv_sec = timeout;
		tval.tv_usec = 0;

		if ((n = select(nfds, &rset, &wset, NULL, timeout ? &tval : NULL)) == 0)
		{
			WSASetLastError(WSAETIMEDOUT);
			return SOCKET_ERROR;
		}
	done:
		ioctlsocket(sockfd, FIONBIO, &zero);
		return 0;
	}

	void PressButton()
	{
		if (!isConnect && !tryConnect)
		{
			WSAStartup(MAKEWORD(2, 0), &WSAData);
			serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
			//unsigned long on = true;
			//int nRet = ioctlsocket(serverSocket, FIONBIO, &on);

			std::wstring title(L"Input IP Here : ");
			SetWindowText(CyanApp::GetHwnd(), title.c_str());

			std::string serverIp = "127.0.0.1";
			//serverIp.assign(wserverIp.begin(), wserverIp.end());

			SOCKADDR_IN serveraddr{};
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
			serveraddr.sin_port = htons(SERVER_PORT);

			retval = connect_nonblock(serverSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr), 5);

			tryConnect = true;
		}
	}
};