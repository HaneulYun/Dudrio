#pragma once
#include "CyanEngine\CyanEngine\framework.h"
//#include "MagentaServer\protocol.h"
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <unordered_map>

#define BUFSIZE 200

using namespace std;

class GuestNetwork : public MonoBehavior<GuestNetwork>
{
private:
	InputField* inputField{ nullptr };
	GameObject* inputIpGuide{ nullptr };

public:
	Text* connectButtonText{ nullptr };

	WSADATA WSAData;
	SOCKET serverSocket;
	int myId;
	int hostId;
	int retval;

	bool isConnect{ false };
	bool tryConnect{ false };
	bool pressButton{ false };

	static GuestNetwork* network;

public:
	GameObject* simsPrefab = NULL;
	GameObject* myCharacter = NULL;

	unordered_map<int, GameObject*> otherCharacters;
	unordered_map<int, GameObject*> sims;

private:
	friend class GameObject;
	friend class MonoBehavior<GuestNetwork>;
	GuestNetwork() = default;
	GuestNetwork(GuestNetwork&) = default;

public:
	~GuestNetwork() {
		isConnect = false;
		closesocket(serverSocket);
		WSACleanup();
	}

	void ProcessPacket(char* ptr);
	void process_data(char* net_buf, size_t io_byte);

	void Receiver();

	void send_packet(void* packet);
	void send_move_start_packet(float xVel, float zVel, float rotAngle, float run_level);
	void send_move_packet(float xVel, float zVel, float rotAngle, float run_level);
	void send_chat_packet(wchar_t msg[]);

	void Login();
	void Logout();

	void Start()
	{
		hostId = -1;
		WSAStartup(MAKEWORD(2, 0), &WSAData);

		inputIpGuide = Scene::scene->CreateUI();
		{
			auto rt = inputIpGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(350, -30, 300, 40);

			Text* text = inputIpGuide->AddComponent<Text>();
			text->text = L"Input Server IP : ";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto inputFieldObject = Scene::scene->CreateImage();
		{
			auto rt = inputFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -30, 300, 40);

			inputField = inputFieldObject->AddComponent<InputField>();
			auto text = inputField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		inputField->gameObject->SetActive(false);
		inputIpGuide->SetActive(false);
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

	void Update()
	{
		if (inputField->gameObject->active)
		{
			if (inputField->isFocused)
			{
				if (Input::GetKeyDown(KeyCode::Return))
				{
					std::string serverIp;
					serverIp.assign(inputField->text.begin(), inputField->text.end());

					SOCKADDR_IN serveraddr{};
					serveraddr.sin_family = AF_INET;
					serveraddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
					serveraddr.sin_port = htons(SERVER_PORT);

					serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
					retval = connect_nonblock(serverSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr), 5);

					PressButton();

					tryConnect = true;
				}
			}
		}

		if (tryConnect)
		{
			if (retval == SOCKET_ERROR)
			{
				tryConnect = false;
				isConnect = false;
			}
			else if (retval == 0)
			{
				connectButtonText->text = L"Logout";
				tryConnect = false;
				isConnect = true;
				unsigned long on = true;
				int nRet = ioctlsocket(serverSocket, FIONBIO, &on);
				Login();
			}
		}
		if(isConnect)
			Receiver();
	}

	void PressButton()
	{
		inputField->clear();

		if (isConnect)
		{
			connectButtonText->text = L"Connect";
			return Logout();
		}

		if (tryConnect)
			return;

		pressButton = !pressButton;
		inputField->gameObject->SetActive(pressButton);
		inputIpGuide->SetActive(pressButton);

		if (pressButton)
			inputField->setFocus(true);
	}
};