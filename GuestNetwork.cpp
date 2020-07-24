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
		
		TerrainGenerator* terrainGenerator = new TerrainGenerator(my_packet->terrainSize, my_packet->terrainSize);
		string fileName = terrainGenerator->createHeightMap(my_packet->frequency, my_packet->octaves, my_packet->seed, (char*)"square");
		delete terrainGenerator;

		GameObject* terrain = Scene::scene->CreateEmpty();
		auto terrainData = terrain->AddComponent<Terrain>();
		{
			{
				wstring name;
				name.assign(fileName.begin(), fileName.end());
				terrainData->terrainData.AlphamapTextureName = name.c_str();
				terrainData->terrainData.heightmapHeight = my_packet->terrainSize;
				terrainData->terrainData.heightmapWidth = my_packet->terrainSize;

				terrainData->terrainData.size = { my_packet->terrainSize, 255, my_packet->terrainSize};

				Graphics::Instance()->commandList->Reset(Graphics::Instance()->commandAllocator.Get(), nullptr);

				terrainData->Set();

				Graphics::Instance()->commandList->Close();
				ID3D12CommandList* cmdsLists[] = { Graphics::Instance()->commandList.Get() };
				Graphics::Instance()->commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
			}
			terrain->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("ground"));
			BuildingBuilder::buildingBuilder->terrain = terrainData;
		}

		myCharacter->transform->Rotate(Vector3{ 0,1,0 }, my_packet->rotAngle);
		auto myc = myCharacter->GetComponent<CharacterMovingBehavior>();
		myc->heightmap = &terrainData->terrainData;
		simsPrefab->GetComponent<CharacterMovingBehavior>()->heightmap = &terrainData->terrainData;
		myc->move(my_packet->xPos, my_packet->zPos);
	}
	break;
	case S2C_LOGIN_FAIL:
	{
		sc_packet_login_fail* my_packet = reinterpret_cast<sc_packet_login_fail*>(ptr);
		isConnect = false;
		tryConnect = false;

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
			otherCharacters[id]->transform->Rotate(Vector3{ 0,1,0 }, my_packet->rotAngle);
			auto oc = otherCharacters[id]->GetComponent<CharacterMovingBehavior>();
			strcpy_s(oc->name, my_packet->name);
			oc->move(my_packet->xPos, my_packet->zPos);
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
	case S2C_CONSTRUCT:
	{
		sc_packet_construct* my_packet = reinterpret_cast<sc_packet_construct*>(ptr);
		Vector2 building_pos{ my_packet->xPos, my_packet->zPos };
		BuildingBuilder::buildingBuilder->build(building_pos, my_packet->angle, my_packet->building_type, my_packet->building_name);
	}
	break;
	case S2C_DESTRUCT:
	{
		sc_packet_destruct* my_packet = reinterpret_cast<sc_packet_destruct*>(ptr);
		//Builder::builder->DestroyBuilding(my_packet->b_inform);
	}
	break;
	case S2C_DESTRUCT_ALL:
	{
		sc_packet_destruct_all* my_packet = reinterpret_cast<sc_packet_destruct_all*>(ptr);
		//Builder::builder->DestroyAllBuilding();
	}
	break;
	case S2C_CHAT:
	{

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
	int t_id = GetCurrentProcessId();
	sprintf_s(l_packet.name, "P%03d", t_id % 1000);
	strcpy_s(myCharacter->GetComponent<CharacterMovingBehavior>()->name, l_packet.name);
	
	send_packet(&l_packet);
}

void GuestNetwork::Logout()
{
	cs_packet_logout l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGOUT;

	send_packet(&l_packet);

	isConnect = false;
	tryConnect = false;

	closesocket(serverSocket);

	//Builder::builder->DestroyAllBuilding();

	hostId = -1;
	for (auto& others : otherCharacters)
		Scene::scene->PushDelete(others.second);

	otherCharacters.clear();
}
