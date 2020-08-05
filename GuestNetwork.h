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
	//GameObject* gameTime{ nullptr };

	// Chat
	InputField* chatField{ nullptr };
	GameObject* chatting[10];
public:
	//Text* connectButtonText{ nullptr };

	WSADATA WSAData;
	SOCKET serverSocket;
	int myId;
	int hostId;
	int retval;
	wchar_t host_name[MAX_ID_LEN + 1];

	bool isConnect{ false };
	bool tryConnect{ false };
	bool pressButton{ false };
	bool flag{ false };

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

		//gameTime = Scene::scene->CreateUI();
		//{
		//	auto rt = gameTime->GetComponent<RectTransform>();
		//	rt->setAnchorAndPivot(0, 1);
		//	rt->setPosAndSize(200, -50, 150, -30);
		//
		//	Text* text = gameTime->AddComponent<Text>();
		//	text->text = GuestGameWorld::gameWorld->convertTimeToText();
		//	text->fontSize = 30;
		//	text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//	text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		//	text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		//}
		//gameTime->SetActive(false);

		auto chatFieldObject = Scene::scene->CreateUI();
		{
			auto rt = chatFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(0, -785, 500, 15);

			chatField = chatFieldObject->AddComponent<InputField>();
			auto text = chatField->Text();
			text->fontSize = 10;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}
		chatField->gameObject->SetActive(true);

		for (int i = 0; i < 10; ++i) {
			chatting[i] = Scene::scene->CreateUI();
			{
				auto rt = chatting[i]->GetComponent<RectTransform>();
				rt->setAnchorAndPivot(0, 1);
				rt->setPosAndSize(0, -785 + ((i + 1) * 15), 500, 15);

				Text* text = chatting[i]->AddComponent<Text>();
				text->text = L"";
				text->fontSize = 10;
				text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}
	}

	void add_chat(wchar_t chatter[], wchar_t chat[])
	{
		for (int i = 8; i >= 0; --i) {
			chatting[i + 1]->GetComponent<Text>()->text = chatting[i]->GetComponent<Text>()->text;
		}
		wstring name = chatter;
		wstring msg = chat;
		chatting[0]->GetComponent<Text>()->text = name + L": " + msg;
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
		if (!isConnect && !flag)
		{
			SOCKADDR_IN serveraddr{};
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(GuestInformConnector::connector->selected_room->serverIP);
			serveraddr.sin_port = htons(GuestInformConnector::connector->selected_room->port_num);

			serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
			retval = connect_nonblock(serverSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr), 5);

			PressButton();

			tryConnect = true;
		}

		if (tryConnect)
		{
			if (retval == SOCKET_ERROR)
			{
				tryConnect = false;
				isConnect = false;
				flag = true;
			}
			else if (retval == 0)
			{
				//connectButtonText->text = L"Logout";
				tryConnect = false;
				isConnect = true;
				flag = true;
				//gameTime->SetActive(true);
				unsigned long on = true;
				int nRet = ioctlsocket(serverSocket, FIONBIO, &on);
				Login();
			}
		}
		if (isConnect)
		{
			//gameTime->GetComponent<Text>()->text = GuestGameWorld::gameWorld->convertTimeToText();
			Receiver();

			if (chatField->isFocused) {
				if (Input::GetKeyDown(KeyCode::Return)) {
					if (chatField->text.size() > MAX_STR_LEN - 1) {
						int oversize = chatField->text.size() - (MAX_STR_LEN - 1);
						for (int i = 0; i < oversize; ++i)
							chatField->text.pop_back();
					}
					send_chat_packet(_wcsdup(chatField->text.c_str()));
					chatField->clear();
					chatField->setFocus(false);
				}
			}
		}
	}

	void PressButton()
	{
		if (isConnect)
		{
			//connectButtonText->text = L"Connect";
			return Logout();
		}

		if (tryConnect)
			return;

		pressButton = !pressButton;
	}
};