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
	GameObject* ip{ nullptr };
	GameObject* ipImage{ nullptr };
	Text* inputIp{ nullptr };

public:
	WSADATA WSAData;
	std::wstring wserverIp;
	SOCKET serverSocket;
	int myId;
	int retval;
	char name[MAX_ID_LEN];
	bool isConnect{ false };
	bool tryConnect{ false };
	bool pressButton{ false };

	float terrainSize;
	float frequency;
	int octaves;
	int seed;

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
	void send_construct_packet(int type, int name, float x, float z, float angle);
	void send_destruct_packet(int type, int name, float x, float z, float angle);
	void send_destruct_all_packet();

	void Login();
	void Logout();

	void Start()
	{
		players.reserve(MAX_USER);
		WSAStartup(MAKEWORD(2, 0), &WSAData);
	}

	void Update()
	{
		if (pressButton)
		{
			if (Input::GetKeyDown(KeyCode::Return) && ip != nullptr && ipImage != nullptr)
			{
				serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
				std::string serverIp;
				serverIp.assign(wserverIp.begin(), wserverIp.end());

				SOCKADDR_IN serveraddr{};
				serveraddr.sin_family = AF_INET;
				serveraddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
				serveraddr.sin_port = htons(SERVER_PORT);

				retval = connect_nonblock(serverSocket, (SOCKADDR*)& serveraddr, sizeof(serveraddr), 5);

				tryConnect = true;
				pressButton = false;

				ip->SetActive(false);
				ipImage->SetActive(false);

				wserverIp.clear();
				Input::ClearBuffer();
			}

			wchar_t str[256];
			wsprintf(str, L"%s", Input::buffer);
			wserverIp = str;
			inputIp->text = wserverIp;
		}
		if (tryConnect)
		{
			if (retval == SOCKET_ERROR)
			{
				tryConnect = false;
				isConnect = false;
				wserverIp.clear();
			}
			else if (retval == 0)
			{
				tryConnect = false;
				isConnect = true;
				wserverIp.clear();
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
		if (!isConnect && !tryConnect && !pressButton)
		{
			Input::ClearBuffer();
			wserverIp.clear();
			if (inputIp != nullptr)
				inputIp->text = wserverIp;
			pressButton = true;

			if (ip == nullptr)
			{
				ip = Scene::scene->CreateUI();
				{
					auto rectTransform = ip->GetComponent<RectTransform>();
					rectTransform->anchorMin = { 0, 1 };
					rectTransform->anchorMax = { 0, 1 };
					rectTransform->pivot = { 0, 1 };
					rectTransform->posX = 350;
					rectTransform->posY = -30;
					rectTransform->width = 300;
					rectTransform->height = 40;

					Text* text = ip->AddComponent<Text>();
					text->text = L"Input Server IP : ";
					text->fontSize = 30;
					text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
					text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
					text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
					Scene::scene->textObjects.push_back(ip);
				}
			}
			else
				ip->SetActive(true);
			if (ipImage == nullptr)
			{
				ipImage = Scene::scene->CreateImage();
				{
					auto rectTransform = ipImage->GetComponent<RectTransform>();
					rectTransform->anchorMin = { 0, 1 };
					rectTransform->anchorMax = { 0, 1 };
					rectTransform->pivot = { 0, 1 };
					rectTransform->posX = 650;
					rectTransform->posY = -30;
					rectTransform->width = 300;
					rectTransform->height = 40;

					{
						auto textobject = ipImage->AddChildUI();
						auto rectTransform = textobject->GetComponent<RectTransform>();
						rectTransform->anchorMin = { 0, 0 };
						rectTransform->anchorMax = { 1, 1 };

						inputIp = textobject->AddComponent<Text>();
						inputIp->text = wserverIp;
						inputIp->fontSize = 30;
						inputIp->color = { 0.0f, 0.0f, 0.0f, 1.0f };
						inputIp->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
						inputIp->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
						Scene::scene->textObjects.push_back(textobject);
					}
				}
			}
			else
				ipImage->SetActive(true);
		}
		else if (pressButton)
		{
			pressButton = false;
			wserverIp.clear();
			if(inputIp != nullptr)
				inputIp->text = wserverIp;
			if (ip != nullptr)
				ip->SetActive(false);
			if (ipImage != nullptr)
				ipImage->SetActive(false);
		}
	}
};