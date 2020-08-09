#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <unordered_map>
#include "GameUI.h"

#define BUFSIZE 200

using namespace std;

class HostNetwork : public MonoBehavior<HostNetwork>
{
private:
	GameObject* chatting[10]{ nullptr };
	//GameObject* gameTime{ nullptr };

public:
	// Chat
	GameUI* gameUI{ nullptr };
	Text* connectButtonText{ nullptr };

	Text* enterMessage{ nullptr };
	float enterMsgTime{ 0 };

	WSADATA WSAData;
	SOCKET lobbySocket;
	SOCKET serverSocket;

	int mainserver_port;
	char mainserver_ip[INET_ADDRSTRLEN];

	int retval;
	int myId;
	wchar_t name[MAX_ID_LEN + 1];
	bool isConnect{ false };
	bool mainConnect{ false };
	bool logouted{ false };
	float terrainSize;
	float frequency;
	int octaves;
	int seed;

	static HostNetwork* network;

public:
	GameObject* simsPrefab[21]{ nullptr };
	unordered_map<int, pair<GameObject*, Village*>> sims;
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
	void send_construct_packet(int type, int name, float x, float z, float angle, int range, bool develop);
	void send_destruct_packet(int type, int name, float x, float z, float angle);
	void send_destruct_all_packet();
	void send_landmark_change_packet(float x, float z, bool change);
	void send_chat_packet(wchar_t msg[]);

	void LobbyLogin();

	void Login();
	void Logout();

	void Start()
	{
		players.reserve(MAX_USER);
		WSAStartup(MAKEWORD(2, 0), &WSAData);

		for (int i = 0; i < 10; ++i) {
			chatting[i] = Scene::scene->CreateUI();
			{
				auto rt = chatting[i]->GetComponent<RectTransform>();
				rt->setAnchorAndPivot(0, 1);
				rt->setPosAndSize(0, -775 + ((i + 1) * 15), 500, 15);

				Text* text = chatting[i]->AddComponent<Text>();
				text->text = L"";
				text->fontSize = 10;
				text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		auto enterMsgObj = Scene::scene->CreateUI();
		{
			auto rt = enterMsgObj->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0.8);
			rt->setPosAndSize(0, 0, 800, 35);

			enterMessage = enterMsgObj->AddComponent<Text>();
			enterMessage->text = L" ";
			enterMessage->font = L"배달의민족 도현";
			enterMessage->fontSize = 25;
			enterMessage->color = { 0.9140625f, 0.796875f, 0.37890625f, 1.0f };
			enterMessage->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			enterMessage->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		enterMsgTime = 5.0f;
	}

	void Update()
	{
		if (logouted){
			for (auto& landmark : HostGameWorld::gameWorld->buildingList) {
				auto iter = landmark.second.find(HostGameWorld::gameWorld->BuildingType::House);
				if (iter != landmark.second.end()) {
					for (auto& house : iter->second) {
						HostGameWorld::gameWorld->addSim(landmark.first, house);
					}
				}
			}
			logouted = false;
		}

		if (isConnect) {
			Receiver();
		}

		if (enterMsgTime > 0.f)
		{
			enterMsgTime -= HostGameWorld::gameWorld->gameDeltaTime;

			if (enterMsgTime < 0.f)
				enterMessage->text = L" ";
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

	void PressButton()
	{
		if (!isConnect) {
			std::string ip_str;
			std::fstream serverIP("ServerIP.txt", std::ios::in);
			serverIP >> ip_str;
			serverIP.close();

			SOCKADDR_IN serveraddr{};
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(ip_str.c_str());
			serveraddr.sin_port = htons(CLIENT_TO_LOBBY_SERVER_PORT);

			lobbySocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
			retval = connect_nonblock(lobbySocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr), 5);

			if (retval == SOCKET_ERROR)
				isConnect = false;
			else if (retval == 0)
			{
				connectButtonText->text = L"로그아웃";
				isConnect = true;
				unsigned long on = true;
				int nRet = ioctlsocket(lobbySocket, FIONBIO, &on);
				LobbyLogin();
			}
		}
		else
		{
			connectButtonText->text = L"오픈하기";
			if (mainConnect)
				return Logout();
		}
	}

	void setEnterMsg(wchar_t* name)
	{
		std::wstring wstr{ name };
		wstr += L"님이 방문했어요!";

		enterMessage->text = wstr.c_str();
		enterMsgTime = 4;
	}
};