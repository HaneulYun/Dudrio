#include "pch.h"
#include "HostNetwork.h"
#include "CharacterMovingBehavior.h"

HostNetwork* HostNetwork::network{ nullptr };

void HostNetwork::ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case LS2C_LOGIN_OK_HOST:
	{
		ls2c_packet_login_ok_host* my_packet = reinterpret_cast<ls2c_packet_login_ok_host*>(ptr);
		strcpy(mainserver_ip, my_packet->serverIP);
		mainserver_port = my_packet->server_port;
		closesocket(lobbySocket);

		SOCKADDR_IN serveraddr{};
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(mainserver_ip);
		serveraddr.sin_port = htons(mainserver_port);

		serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
		retval = connect_nonblock(serverSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr), 10);
		if (retval == SOCKET_ERROR)
		{
			connectButtonText->text = L"Open";
			pressButton = false;
			tryConnect = false;
			isConnect = false;

			closesocket(serverSocket);
		}
		else if (retval == 0)
		{
			connectButtonText->text = L"Logout";
			tryConnect = false;
			isConnect = true;
			mainConnect = true;
			unsigned long on = true;
			int nRet = ioctlsocket(serverSocket, FIONBIO, &on);
			Login();
		}
	}
	break;
	case LS2C_LOGIN_FAIL:
	{
		ls2c_pakcet_login_fail* my_packet = reinterpret_cast<ls2c_pakcet_login_fail*>(ptr);

		connectButtonText->text = L"Open";
		pressButton = false;
		isConnect = false;
		tryConnect = false;

		closesocket(lobbySocket);
	}
	break;
	case S2C_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		myId = my_packet->id;
		HostGameWorld::gameWorld->gameTime = my_packet->game_time;

		// 심 초기화
		for (auto& sims : HostGameWorld::gameWorld->simList){
			Scene::scene->PushDelete(sims.second);
		}
		HostGameWorld::gameWorld->simList.clear();

		// 빌딩 정보 전송
		for (auto& p : HostGameWorld::gameWorld->buildingList)
			for(auto& q: p.second)
				for (auto& r : q.second) {
					Vector3 building_forward = r->transform->forward;
					building_forward.y = 0;
					building_forward.Normalize();
					Vector3 forward = { 0,0,1 };
					float angle = Vector3::DotProduct(forward, building_forward);
					Vector3 dir = Vector3::CrossProduct(forward, building_forward);
					angle = XMConvertToDegrees(acos(angle));
					angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;

					int range = 0;
					if (r->GetComponent<Village>() != nullptr)
						range = r->GetComponent<Village>()->radiusOfLand;
					send_construct_packet(q.first, r->GetComponent<Building>()->index, r->transform->position.x, r->transform->position.z, angle, range);
				}
	}
	break;
	case S2C_LOGIN_FAIL:
	{
		sc_packet_login_fail* my_packet = reinterpret_cast<sc_packet_login_fail*>(ptr);
		
		connectButtonText->text = L"Open";
		pressButton = false;
		isConnect = false;
		tryConnect = false;
		mainConnect = false;

		closesocket(serverSocket);
	}
	break;
	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;

		if (id != myId) {
			auto player = gameObject->scene->Duplicate(simsPrefab);
			auto behavior = player->GetComponent<CharacterMovingBehavior>();
			behavior->move(my_packet->xPos, my_packet->zPos, my_packet->rotAngle);
			wcscpy_s(behavior->name, my_packet->name);
			auto uiPos = player->AddChild();
			{
				uiPos->transform->position = { 0, 1.8, 0 };
				auto uiBox = uiPos->AddChildUI();
				{
					auto rt = uiBox->GetComponent<RectTransform>();
					rt->renderMode = RectTransform::WorldSpace;

					std::wstring str(behavior->name, &behavior->name[strlen(behavior->name)]);
					auto text = uiBox->AddComponent<Text>();
					text->text = str;
					text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
					text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				}
			}

			players[id] = player;
		}
	}
	break;
	case S2C_MOVE:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int id = my_packet->id;

		if (id != myId) {
			if (0 != players.count(id)) {
				auto p = players[id]->GetComponent<CharacterMovingBehavior>();
				p->add_move_queue({ my_packet->xPos, 0, my_packet->zPos }, my_packet->rotAngle);
			}
		}
	}
	break;
	case S2C_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == myId) {
			Logout();
		}
		else {
			if (0 != players.count(other_id))
			{
				Scene::scene->PushDelete(players[other_id]);
				players.erase(other_id);
			}
		}
	}
	break;
	case S2C_SIM_ENTER:
	{
		sc_packet_sim_enter* my_packet = reinterpret_cast<sc_packet_sim_enter*>(ptr);
		int id = my_packet->id;

		sims[id] = gameObject->scene->Duplicate(simsPrefab);
		auto p = sims[id]->GetComponent<CharacterMovingBehavior>();
		p->move(my_packet->xPos, my_packet->zPos, my_packet->rotAngle);
	}
	break;
	case S2C_SIM_MOVE:
	{
		sc_packet_sim_move* my_packet = reinterpret_cast<sc_packet_sim_move*>(ptr);
		int id = my_packet->id;

		if (0 != sims.count(id)) {
			auto p = sims[id]->GetComponent<CharacterMovingBehavior>();
			p->add_move_queue({ my_packet->xPos, 0, my_packet->zPos }, my_packet->rotAngle);
		}
	}
	break;
	case S2C_SIM_LEAVE:
	{
		sc_packet_sim_leave* my_packet = reinterpret_cast<sc_packet_sim_leave*>(ptr);
		int id = my_packet->id;

		if (0 != sims.count(id))
		{
			Scene::scene->PushDelete(sims[id]);
			sims.erase(id);
		}
	}
	break;

	case S2C_CONSTRUCT:
	{
		sc_packet_construct* my_packet = reinterpret_cast<sc_packet_construct*>(ptr);
		Vector2 building_pos{ my_packet->xPos, my_packet->zPos };
		GameObject* my_landmark;
		for (auto landmark : HostGameWorld::gameWorld->buildingList) {
			Vector3 landPos = landmark.first->transform->position;
			float range = landmark.first->GetComponent<Village>()->radiusOfLand;
			float dist = sqrt(pow(building_pos.x - landPos.x, 2) + pow(building_pos.y - landPos.z, 2));
			if (range >= dist) {
				my_landmark = landmark.first;
				break;
			}
		}
		BuildingBuilder::buildingBuilder->build(building_pos, my_packet->angle, my_packet->building_type, my_packet->building_name, my_landmark);
	}
		break;
	case S2C_DESTRUCT:
		break;
	case S2C_DESTRUCT_ALL:
		break;
	case S2C_GAME_TIME:
	{
		sc_packet_game_time* my_packet = reinterpret_cast<sc_packet_game_time*>(ptr);
		HostGameWorld::gameWorld->gameTime = my_packet->game_time;
	}
		break;
	case S2C_CHAT:
	{
		sc_packet_chat* my_packet = reinterpret_cast<sc_packet_chat*>(ptr);
		int id = my_packet->id;
		if (id == myId)
		{
			add_chat(name, my_packet->mess);
		}
		else
		{
			add_chat(players[id]->GetComponent<CharacterMovingBehavior>()->name, my_packet->mess);
		}
	}
		break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void HostNetwork::process_data(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUFSIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = (unsigned char)ptr[0];
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

void HostNetwork::Receiver()
{
	char net_buf[BUFSIZE];

	int ret = 0;
	if (mainConnect)
		ret = recv(serverSocket, net_buf, BUFSIZE, 0);
	else 
		ret = recv(lobbySocket, net_buf, BUFSIZE, 0);

	if (ret > 0)	process_data(net_buf, ret);
}

void HostNetwork::send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);

	if(mainConnect)
		send(serverSocket, p, (unsigned char)p[0], 0);
	else
		send(lobbySocket, p, (unsigned char)p[0], 0);
}

void HostNetwork::send_construct_packet(int type, int name, float x, float z, float angle, int land_range)
{
	cs_packet_construct m_packet;
	m_packet.type = C2S_CONSTRUCT;
	m_packet.size = sizeof(m_packet);
	m_packet.building_type = type;
	m_packet.building_name = name;
	m_packet.xpos = x;
	m_packet.zpos = z;
	m_packet.angle = angle;
	m_packet.landmark_range = land_range;

	send_packet(&m_packet);
}

void HostNetwork::send_destruct_packet(int type, int name, float x, float z, float angle)
{
	cs_packet_destruct m_packet;
	m_packet.type = C2S_DESTRUCT;
	m_packet.size = sizeof(m_packet);
	m_packet.building_type = type;
	m_packet.building_name = name;
	m_packet.xPos = x;
	m_packet.zPos = z;
	m_packet.angle = angle;

	send_packet(&m_packet);
}

void HostNetwork::send_destruct_all_packet()
{
	cs_packet_destruct_all m_packet;
	m_packet.type = C2S_DESTRUCT_ALL;
	m_packet.size = sizeof(m_packet);

	send_packet(&m_packet);
}

void HostNetwork::send_chat_packet(wchar_t msg[])
{
	cs_packet_chat m_packet;
	m_packet.type = C2S_CHAT;
	m_packet.size = sizeof(m_packet);
	wcscpy_s(m_packet.message, msg);

	send_packet(&m_packet);
}

void HostNetwork::Login()
{
	cs_packet_login_host l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGIN_HOST;
	wcscpy_s(l_packet.name, name);
	HostGameWorld::gameWorld->timeSpeed = HostGameWorld::gameWorld->TimeSpeed::X1;
	l_packet.game_time = HostGameWorld::gameWorld->gameTime;
	l_packet.frequency = frequency;
	l_packet.octaves = octaves;
	l_packet.seed = seed;
	l_packet.terrainSize = terrainSize;

	send_packet(&l_packet);
}

void HostNetwork::Logout()
{
	cs_packet_logout l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGOUT;

	send_packet(&l_packet);

	pressButton = false;
	isConnect = false;
	tryConnect = false;
	mainConnect = false;
	logouted = true;

	connectButtonText->text = L"Open";
	closesocket(serverSocket);

	for (auto& p : players)
		Scene::scene->PushDelete(p.second);
	players.clear();

	for (auto& p : sims)
		Scene::scene->PushDelete(p.second);
	sims.clear();
}


void HostNetwork::LobbyLogin()
{
	c2ls_packet_login_host l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2LS_LOGIN_HOST;
	wcscpy_s(l_packet.name, name);
	l_packet.terrain_size = terrainSize;
	l_packet.frequency = frequency;
	l_packet.octaves = octaves;
	l_packet.seed = seed;

	send_packet(&l_packet);
}
