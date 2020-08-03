#include "pch.h"
#include "GuestNetwork.h"
#include "CharacterController.h"
#include "CharacterMovingBehavior.h"

GuestNetwork* GuestNetwork::network{ nullptr };

void GuestNetwork::ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case S2C_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		myId = my_packet->id;
		GuestGameWorld::gameWorld->gameTime = my_packet->game_time;
		GuestGameWorld::gameWorld->timeSpeed = GuestGameWorld::gameWorld->TimeSpeed::X1;

		// 호스트 이름 저장
		hostId = my_packet->host_id;
		strcpy_s(host_name, my_packet->host_name);

		// 내 캐릭터 정보 지정
		auto myc = myCharacter->GetComponent<CharacterMovingBehavior>();
		myc->move(my_packet->xPos, my_packet->zPos, my_packet->rotAngle);
	}
	break;
	case S2C_LOGIN_FAIL:
	{
		sc_packet_login_fail* my_packet = reinterpret_cast<sc_packet_login_fail*>(ptr);

		connectButtonText->text = L"Connect";
		pressButton = false;
		isConnect = false;
		tryConnect = false;
		gameTime->SetActive(false);

		closesocket(serverSocket);
	}
	break;
	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		int o_type = my_packet->o_type;

		if (id != myId && o_type == O_GUEST){
			otherCharacters[id] = gameObject->scene->Duplicate(simsPrefab);
			auto oc = otherCharacters[id]->GetComponent<CharacterMovingBehavior>();
			strcpy_s(oc->name, my_packet->name);
			oc->move(my_packet->xPos, my_packet->zPos, my_packet->rotAngle);
		}
	}
	break;
	case S2C_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int id = my_packet->id;

		if (id != hostId && id != myId) {
			if (0 != otherCharacters.count(id)) {
				Scene::scene->PushDelete(otherCharacters[id]);
				otherCharacters.erase(id);
			}
		}
		else {
			Logout();
		}
	}
	break;
	case S2C_MOVE:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int id = my_packet->id;
		if (id == myId){
			auto myc = myCharacter->GetComponent<CharacterMovingBehavior>();
			auto mycPos = myCharacter->transform->position;
			if (Vector3{ my_packet->xPos, 0, my_packet->zPos } != Vector3{ mycPos.x, 0, mycPos.z })
				myc->add_move_queue({ my_packet->xPos, 0, my_packet->zPos }, 0);
		}
		else if (id != hostId) {
			if (0 != otherCharacters.count(id)){
				auto oc = otherCharacters[id]->GetComponent<CharacterMovingBehavior>();
				oc->add_move_queue({ my_packet->xPos, 0, my_packet->zPos }, my_packet->rotAngle);
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
		BuildingBuilder::buildingBuilder->guestBuild(my_packet->building_type, my_packet->building_name, my_packet->xPos, my_packet->zPos, my_packet->angle, my_packet->landmark_range);
	}
	break;
	case S2C_DESTRUCT:
	{
		sc_packet_destruct* my_packet = reinterpret_cast<sc_packet_destruct*>(ptr);
		Vector3 p_pos{ my_packet->xPos,0, my_packet->zPos };
		GameObject* my_landmark;
		GameObject* dest_obj;
		for (auto landmark : GuestGameWorld::gameWorld->buildingList) {
			Vector3 landPos = landmark.first->transform->position;
			float range = landmark.first->GetComponent<Village>()->radiusOfLand;
			float dist = sqrt(pow(p_pos.x - landPos.x, 2) + pow(p_pos.z - landPos.z, 2));
			if (range >= dist) {
				my_landmark = landmark.first;
				for (auto type : landmark.second) {
					if (type.first != my_packet->building_type)
						continue;
					for (auto building : type.second) {
						Vector3 b_pos { building->transform->position.x, 0, building->transform->position.z };
						if (b_pos == p_pos) {
							GuestGameWorld::gameWorld->deleteInGameWorld(my_landmark, building, my_packet->building_type, my_packet->building_name);
							return;
						}
					}
				}
			}
		}
	}
	break;
	case S2C_DESTRUCT_ALL:
	{
		sc_packet_destruct_all* my_packet = reinterpret_cast<sc_packet_destruct_all*>(ptr);
		for (auto landmark : GuestGameWorld::gameWorld->buildingList) {
			for (auto type : landmark.second) {
				for (auto building : type.second) {
					if(landmark.first != building)
						Scene::scene->PushDelete(building);
				}
				type.second.clear();
			}
			landmark.second.clear();
			Scene::scene->PushDelete(landmark.first);
		}
		GuestGameWorld::gameWorld->buildingList.clear();
	}
	break;
	case S2C_CHAT:
	{
		sc_packet_chat* my_packet = reinterpret_cast<sc_packet_chat*>(ptr);
		int id = my_packet->id;
		if (id == myId){
			wstring wname;
			string cname = myCharacter->GetComponent<CharacterMovingBehavior>()->name;
			wname.assign(cname.begin(), cname.end());
			add_chat(_wcsdup(wname.c_str()), my_packet->mess);
		}
		else if (id == hostId){
			wstring wname;
			string cname = host_name;
			wname.assign(cname.begin(), cname.end());
			add_chat(_wcsdup(wname.c_str()), my_packet->mess);
		}
		else{
			wstring wname;
			string cname = otherCharacters[id]->GetComponent<CharacterMovingBehavior>()->name;
			wname.assign(cname.begin(), cname.end());
			add_chat(_wcsdup(wname.c_str()), my_packet->mess);
		}
	}
	break;
	case S2C_GAME_TIME:
	{
		sc_packet_game_time* my_packet = reinterpret_cast<sc_packet_game_time*>(ptr);
		GuestGameWorld::gameWorld->gameTime = my_packet->game_time;
	}
	break; 
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void GuestNetwork::process_data(char* net_buf, size_t io_byte)
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

void GuestNetwork::Receiver()
{
	char net_buf[BUFSIZE];
	auto retval = recv(serverSocket, net_buf, BUFSIZE, 0);

	if (retval > 0)	process_data(net_buf, retval);
}

void GuestNetwork::send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);
	send(serverSocket, p, (unsigned char)p[0], 0);
}

void GuestNetwork::send_move_start_packet(float xVel, float zVel, float rotAngle, float run_level)
{
	cs_packet_move_start m_packet;
	m_packet.type = C2S_MOVE_START;
	m_packet.size = sizeof(m_packet);
	m_packet.xVel = xVel;
	m_packet.zVel = zVel;
	m_packet.rotAngle = rotAngle;
	m_packet.run_level = run_level;

	send_packet(&m_packet);
}

void GuestNetwork::send_move_packet(float xVel, float zVel, float rotAngle, float run_level)
{
	cs_packet_move m_packet;
	m_packet.type = C2S_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.xVel = xVel;
	m_packet.zVel = zVel;
	m_packet.rotAngle = rotAngle;
	m_packet.run_level = run_level;

	send_packet(&m_packet);
}

void GuestNetwork::send_chat_packet(wchar_t msg[])
{
	cs_packet_chat m_packet;
	m_packet.type = C2S_CHAT;
	m_packet.size = sizeof(m_packet);
	wcscpy_s(m_packet.message, msg);

	send_packet(&m_packet);
}

void GuestNetwork::Login()
{
	cs_packet_login_guest l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGIN_GUEST;
	strcpy_s(l_packet.name, myCharacter->GetComponent<CharacterMovingBehavior>()->name);
	
	send_packet(&l_packet);
}

void GuestNetwork::Logout()
{
	cs_packet_logout l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGOUT;

	send_packet(&l_packet);

	connectButtonText->text = L"Connect";
	pressButton = false;
	isConnect = false;
	tryConnect = false;
	gameTime->SetActive(false);

	closesocket(serverSocket);

	for (auto landmark : GuestGameWorld::gameWorld->buildingList) {
		for (auto type : landmark.second) {
			for (auto building : type.second) {
				if (landmark.first != building)
					Scene::scene->PushDelete(building);
			}
			type.second.clear();
		}
		landmark.second.clear();
		Scene::scene->PushDelete(landmark.first);
	}

	GuestGameWorld::gameWorld->buildingList.clear();
	hostId = -1;
	for (auto& sim : sims)
		Scene::scene->PushDelete(sim.second);
	sims.clear();

	for (auto& others : otherCharacters)
		Scene::scene->PushDelete(others.second);
	otherCharacters.clear();
}
