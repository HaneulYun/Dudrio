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
		myCharacter->transform->Rotate(Vector3{ 0,1,0 }, my_packet->rotAngle);
		auto myc = myCharacter->GetComponent<CharacterMovingBehavior>();
		myc->move(my_packet->xPos, my_packet->zPos);
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
	case S2C_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int id = my_packet->id;

		if(id != hostId && id != myId) {
			if (0 != otherCharacters.count(id)){
				Scene::scene->PushDelete(otherCharacters[id]);
				otherCharacters.erase(id);
			}
		}
		else {
			//Builder::builder->DestroyAllBuilding();

			hostId = -1;
			for (auto& others : otherCharacters)
				Scene::scene->PushDelete(others.second);
			
			otherCharacters.clear();
		}
	}
	break;
	case S2C_CONSTRUCT:
	{
		sc_packet_construct* my_packet = reinterpret_cast<sc_packet_construct*>(ptr);
		BuildingBuilder::buildingBuilder->enterBuildMode(0, 0);
		BuildingBuilder::buildingBuilder->build({ my_packet->b_inform.xPos, my_packet->b_inform.yPos, my_packet->b_inform.zPos });
		//Builder::builder->BuildNewBuilding(my_packet->b_inform);
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
		if (0 == in_packet_size) in_packet_size = ptr[0];
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
	send(serverSocket, p, p[0], 0);
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

void GuestNetwork::Login()
{
	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGIN;
	int t_id = GetCurrentProcessId();
	sprintf_s(l_packet.name, "P%03d", t_id % 1000);
	strcpy_s(myCharacter->GetComponent<CharacterMovingBehavior>()->name, l_packet.name);
	send_packet(&l_packet);
}