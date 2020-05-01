#include "pch.h"
#include "GuestNetwork.h"
#include "CharacterController.h"
#include "CharacterMovingBehavior.h"

void GuestNetwork::ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case S2C_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		myId = my_packet->id;
		auto myc = myCharacter->GetComponent<CharacterMovingBehavior>();
		myc->velocity = Vector3{ my_packet->xMove, 0.0, my_packet->zMove };
		myc->move(Vector3(my_packet->x, 0.0f, my_packet->z));
	}
	break;

	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		int o_type = my_packet->o_type;

		if (id == myId) {
			auto myc = myCharacter->GetComponent<CharacterMovingBehavior>();
			myc->velocity = Vector3{ my_packet->xMove, 0.0, my_packet->zMove };
			myc->move(Vector3(my_packet->x, 0.0f, my_packet->z));
		}
		else if (o_type == O_GUEST){
			otherCharacters[id] = gameObject->scene->Duplicate(simsPrefab);
			strcpy_s(otherCharacters[id]->GetComponent<CharacterMovingBehavior>()->name, my_packet->name);
			auto oc = otherCharacters[id]->GetComponent<CharacterMovingBehavior>();
			oc->velocity = Vector3{ my_packet->xMove, 0.0, my_packet->zMove };
			oc->move(Vector3(my_packet->x, 0.0f, my_packet->z));
		}
		else
		{
			// HOST일 경우 처리
			hostId = id;
		}
	}
	break;
	case S2C_MOVE:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;

		if (other_id == myId) {
			auto myc = myCharacter->GetComponent<CharacterMovingBehavior>();
			myc->velocity = Vector3{ my_packet->xMove, 0.0, my_packet->zMove };
			myc->move(Vector3(my_packet->x, 0.0f, my_packet->z));
		}
		else if(other_id != hostId) {
			if (0 != otherCharacters.count(other_id))
			{
				auto oc = otherCharacters[other_id]->GetComponent<CharacterMovingBehavior>();
				oc->velocity = Vector3{ my_packet->xMove, 0.0, my_packet->zMove };
				oc->move(Vector3(my_packet->x, 0.0f, my_packet->z));
			}
		}
	}
	break;

	case S2C_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
	
		if (other_id == myId) {
			Scene::scene->PushDelete(myCharacter);
		}
		else if(other_id != hostId) {
			if (0 != otherCharacters.count(other_id))
			{
				Scene::scene->PushDelete(otherCharacters[other_id]);
				otherCharacters.erase(other_id);
			}
		}
		else 
		{
			// HOST라면 모든 건물 삭제 및 모든 캐릭터 삭제
			for (auto& others : otherCharacters)
			{
				Scene::scene->PushDelete(others.second);
				otherCharacters.erase(others.first);
			}
		}
	}
	break;

	case S2C_CONSTRUCT:
	{
		sc_packet_construct* my_packet = reinterpret_cast<sc_packet_construct*>(ptr);
		int other_id = my_packet->id;

		if (other_id == hostId)
		{
			BuildingInform b_inform = my_packet->b_inform;
			Builder::builder->BuildNewBuilding(b_inform.buildingType, Vector3(b_inform.xPos, b_inform.yPos, b_inform.zPos), b_inform.rotAngle);
		}
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

void GuestNetwork::send_move_packet(float xMove, float zMove)//unsigned char dir)
{
	cs_packet_move m_packet;
	m_packet.type = C2S_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.xMove = xMove;
	m_packet.zMove = zMove;
	//m_packet.direction = dir;
	send_packet(&m_packet);
}

void GuestNetwork::Login()
{
	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGIN;
	int t_id = GetCurrentProcessId();
	sprintf_s(l_packet.name, "P%03d", t_id % 1000);
	myCharacter = gameObject->scene->Duplicate(simsPrefab);
	myCharacter->AddComponent<CharacterController>()->network = this;
	strcpy_s(myCharacter->GetComponent<CharacterMovingBehavior>()->name, l_packet.name);
	send_packet(&l_packet);
}