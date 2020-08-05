#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <unordered_set>

#define BUFSIZE 200

struct RoomInfo {
	int room_id;
	wchar_t name[MAX_ID_LEN + 1];
	char serverIP[INET_ADDRSTRLEN];
	int port_num;
	int terrain_size;
	int frequency;
	int octaves;
	int seed;

	RoomInfo() {}
	RoomInfo(int id, wchar_t* h_name, char* ip, int port, int t_size, int freq, int oct, int sd)
	{
		room_id = id;
		wcscpy_s(name, h_name);
		strcpy_s(serverIP, ip);
		port_num = port;
		terrain_size = t_size;
		frequency = freq;
		octaves = oct;
		seed = sd;
	}
};

class GuestInformConnector : public MonoBehavior<GuestInformConnector>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	// page 1
	InputField* nameField{ nullptr };
	GameObject* nameGuide{ nullptr };

	InputField* lobbyServerIPField{ nullptr };
	GameObject* lobbyServerIPGuide{ nullptr };

	GameObject* characterCustomizePrevButton{ nullptr };
	GameObject* characterCustomizeNextButton{ nullptr };

	GameObject* SetMyCharacterbutton{ nullptr };

	// page 2
	GameObject* prevButton{ nullptr };	// 뒤로가기 버튼

	// Network
	SOCKET lobbySocket;
	bool is_connect{ false };

public  /*이 영역에 public 변수를 선언하세요.*/:
	GameObject* background{ nullptr };
	GameObject* buttonPrefab{ nullptr };

	// 게스트 이름
	static wchar_t name[MAX_ID_LEN + 1];

	// 선택한 방 정보
	std::unordered_map<int, std::pair<GameObject*, RoomInfo*>> hosts;
	static RoomInfo* selected_room;

	static GuestInformConnector* connector;
private:
	friend class GameObject;
	friend class MonoBehavior<GuestInformConnector>;
	GuestInformConnector() = default;
	GuestInformConnector(GuestInformConnector&) = default;

public:
	~GuestInformConnector() {}

	bool checkRangeInField()
	{
		if (nameField->text.length() > MAX_ID_LEN || nameField->text.length() < 2)
			return false;
		return true;
	}

	void clearFields()
	{
		nameField->clear();
		lobbyServerIPField->clear();
	}

	bool insertInform()
	{
		if (!checkRangeInField())
			return false;
		wcsncpy(name, nameField->text.c_str(), nameField->text.length());

		clearFields();
		return true;
	}

	void backToPrevPage()
	{
		background->GetComponent<RectTransform>()->anchorMin = { 0.5, 0 };
		SetField(true);
		clearFields();
		
		for (auto host : hosts)
			Scene::scene->PushDelete(host.second.first);
		hosts.clear();
		// closesocket 하자
	}

	void goToNextPage()
	{
		SetField(false);
		clearFields();

		background->GetComponent<RectTransform>()->anchorMin = { 0, 0 };
	}

	void SetField(bool flag)
	{
		nameField->gameObject->SetActive(flag);
		nameGuide->SetActive(flag);

		lobbyServerIPField->gameObject->SetActive(flag);
		lobbyServerIPGuide->SetActive(flag);

		characterCustomizePrevButton->SetActive(flag);
		characterCustomizeNextButton->SetActive(flag);

		SetMyCharacterbutton->SetActive(flag);

		// ----------------------------------------

		prevButton->SetActive(!flag);
	}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		prevButton = Scene::scene->CreateImage();
		{
			auto rt = prevButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(30, -50, 40, 40);

			prevButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					connector->backToPrevPage();
				});
			{
				auto textobject = prevButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"<-";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		characterCustomizePrevButton = Scene::scene->CreateImage();
		{
			auto rt = characterCustomizePrevButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(30, -380, 40, 40);

			characterCustomizePrevButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					// 커스터마이징 버튼
				});
			{
				auto textobject = characterCustomizePrevButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"<-";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		characterCustomizeNextButton = Scene::scene->CreateImage();
		{
			auto rt = characterCustomizeNextButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(530, -380, 40, 40);

			characterCustomizeNextButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					// 커스터마이징 버튼
				});
			{
				auto textobject = characterCustomizeNextButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"->";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		nameGuide = Scene::scene->CreateUI();
		{
			auto rt = nameGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -50, 300, 40);

			Text* text = nameGuide->AddComponent<Text>();
			text->text = L"Input your name";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto nameFieldObject = Scene::scene->CreateImage();
		{
			auto rt = nameFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -100, 300, 40);

			nameField = nameFieldObject->AddComponent<InputField>();
			auto text = nameField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		lobbyServerIPGuide = Scene::scene->CreateUI();
		{
			auto rt = lobbyServerIPGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -350, 500, 40);

			Text* text = lobbyServerIPGuide->AddComponent<Text>();
			text->text = L"Input lobby server's IP";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto lobbyServerIPFieldObject = Scene::scene->CreateImage();
		{
			auto rt = lobbyServerIPFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -400, 300, 40);

			lobbyServerIPField = lobbyServerIPFieldObject->AddComponent<InputField>();
			auto text = lobbyServerIPField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		SetMyCharacterbutton = Scene::scene->CreateImage();
		{
			auto rt = SetMyCharacterbutton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(970, -700, 200, 70);

			SetMyCharacterbutton->AddComponent<Button>()->AddEvent(
				[](void*) {
					connector->login();
				});
			{
				auto textobject = SetMyCharacterbutton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Set my character";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}
		SetField(true);
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		if (is_connect) {
			Receiver();

			int i = 0;
			int j = 0;
			for (auto host : hosts) {
				if (i == 4) {
					i = 0;
					j++;
				}
				host.second.first->GetComponent<RectTransform>()->setPosAndSize(50 + (i * 260), -50 - (j * 50), 100, 40);
				i++;
			}
		}
	}

	void process_packet(char* ptr)
	{
		switch (ptr[1])
		{
		case LS2C_LOGIN_OK_GUEST:
		{
			ls2c_packet_login_ok_guest* my_packet = reinterpret_cast<ls2c_packet_login_ok_guest*>(ptr);
			goToNextPage();
		}
		break;
		case LS2C_NEW_ROOM:
		{
			ls2c_pakcet_new_room* my_packet = reinterpret_cast<ls2c_pakcet_new_room*>(ptr);
			RoomInfo* room = new RoomInfo({ my_packet->room_id, my_packet->host_name, my_packet->serverIP, my_packet->server_port,
					my_packet->terrain_size, my_packet->frequency, my_packet->octaves, my_packet->seed });


			auto button = Scene::scene->Duplicate(buttonPrefab);
			button->layer = (int)RenderLayer::UI;

			button->children.front()->GetComponent<Text>()->text = my_packet->host_name;
			button->AddComponent<Button>()->AddEvent(
				[](void* ptr) {
					connector->selected_room = reinterpret_cast<RoomInfo*>(ptr);
					connector->is_connect = false;
					closesocket(connector->lobbySocket);
					WSACleanup();
					SceneManager::LoadScene("GuestScene");
				}, room);

			hosts[my_packet->room_id] = std::make_pair(button, room);
		}
		break;
		case LS2C_DELETE_ROOM:
		{
			ls2c_packet_delete_room* my_packet = reinterpret_cast<ls2c_packet_delete_room*>(ptr);
			if (hosts.count(my_packet->room_id) != 0) {
				Scene::scene->PushDelete(hosts[my_packet->room_id].first);
				delete hosts[my_packet->room_id].second;
				hosts.erase(my_packet->room_id);
			}
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
			if (0 == in_packet_size) in_packet_size = (unsigned char)ptr[0];
			if (io_byte + saved_packet_size >= in_packet_size) {
				memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
				process_packet(packet_buffer);
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
		int ret = recv(lobbySocket, net_buf, BUFSIZE, 0);

		if (ret > 0) process_data(net_buf, ret);
	}

	void login()
	{
		WSAData WSAData;
		WSAStartup(MAKEWORD(2, 0), &WSAData);
		if (!connector->insertInform())
			return;

		SOCKADDR_IN serveraddr{};
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(LOBBY_SERVER_IP);
		serveraddr.sin_port = htons(CLIENT_TO_LOBBY_SERVER_PORT);

		lobbySocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
		int retval = connect_nonblock(lobbySocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr), 5);
		
		if (retval == 0)
		{
			unsigned long on = true;
			int nRet = ioctlsocket(lobbySocket, FIONBIO, &on);

			c2ls_packet_login_guest l_packet;
			l_packet.size = sizeof(l_packet);
			l_packet.type = C2LS_LOGIN_GUEST;
			wcscpy_s(l_packet.name, name);

			send_packet(&l_packet);
			is_connect = true;
		}
		else {
			closesocket(lobbySocket);
			WSACleanup();
			is_connect = false;
		}
	}

	void send_packet(void* packet)
	{
		char* p = reinterpret_cast<char*>(packet);
		send(lobbySocket, p, (unsigned char)p[0], 0);
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

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};