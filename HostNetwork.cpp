#include "pch.h"
#include "HostNetwork.h"
#include "CharacterMovingBehavior.h"

HostNetwork* HostNetwork::network{ nullptr };

void HostNetwork::ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case S2C_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		myId = my_packet->id;
	}
	break;

	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;

		if (id == myId) {
		
		}
		else {
			players[id] = gameObject->scene->Duplicate(simsPrefab);
			strcpy_s(players[id]->GetComponent<CharacterMovingBehavior>()->name, my_packet->name);
			auto p = players[id]->GetComponent<CharacterMovingBehavior>();
			p->velocity = Vector3{ my_packet->xMove, 0.0, my_packet->zMove };
			p->move(my_packet->x, my_packet->z);
		}
	}
	break;
	case S2C_MOVE_START:
	{
		sc_packet_move_start* my_packet = reinterpret_cast<sc_packet_move_start*>(ptr);
		int id = my_packet->id;
		if (id != myId) {
			if (0 != players.count(id))
			{
				auto oc = players[id]->GetComponent<CharacterMovingBehavior>();
				oc->velocity = Vector3{ my_packet->xMove, 0.0, my_packet->zMove };
				oc->move(my_packet->x, my_packet->z);
				oc->moving = true;
			}
		}
	}
	break;
	case S2C_MOVE_END:
	{
		sc_packet_move_end* my_packet = reinterpret_cast<sc_packet_move_end*>(ptr);
		int id = my_packet->id;
		if (id != myId) {
			if (0 != players.count(id))
			{
				auto oc = players[id]->GetComponent<CharacterMovingBehavior>();
				oc->velocity = Vector3{ 0.0, 0.0, 0.0 };
				oc->move(my_packet->x, my_packet->z);
				oc->moving = false;
			}
		}
	}
	break;
	case S2C_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == myId) {
			for (auto& p : players)
			{
				Scene::scene->PushDelete(p.second);
			}
			players.clear();
			closesocket(serverSocket);
			isConnect = false;
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

	case S2C_CONSTRUCT:
		break;
	case S2C_DESTRUCT:
		break;
	case S2C_DESTRUCT_ALL:
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

void HostNetwork::Receiver()
{
	char net_buf[BUFSIZE];
	auto retval = recv(serverSocket, net_buf, BUFSIZE, 0);

	if (retval > 0)	process_data(net_buf, retval);
}

void HostNetwork::send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);
	send(serverSocket, p, p[0], 0);
}

void HostNetwork::send_construct_packet(BuildingInform b_inform)
{
	cs_packet_construct m_packet;
	m_packet.type = C2S_CONSTRUCT;
	m_packet.size = sizeof(m_packet);
	m_packet.b_inform = b_inform;

	send_packet(&m_packet);
}

void HostNetwork::send_destruct_packet(BuildingInform b_inform)
{
	cs_packet_destruct m_packet;
	m_packet.type = C2S_DESTRUCT;
	m_packet.size = sizeof(m_packet);
	m_packet.b_inform = b_inform;

	send_packet(&m_packet);
}

void HostNetwork::send_destruct_all_packet()
{
	cs_packet_destruct_all m_packet;
	m_packet.type = C2S_DESTRUCT_ALL;
	m_packet.size = sizeof(m_packet);

	send_packet(&m_packet);
}

void HostNetwork::Login()
{
	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGIN_HOST;
	int t_id = GetCurrentProcessId();
	sprintf_s(l_packet.name, "P%03d", t_id % 1000);
	strcpy_s(name, l_packet.name);
	send_packet(&l_packet);

	for (auto& p : BuildManager::buildManager->buildings)
	{
		send_construct_packet(p.first);
	}
}