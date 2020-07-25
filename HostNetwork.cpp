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

		//for (auto& p : BuildManager::buildManager->buildings)
		//	send_construct_packet(p.first);
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

		if (id != myId) {
			players[id] = gameObject->scene->Duplicate(simsPrefab);
			players[id]->transform->Rotate(Vector3{ 0.0f, 1.0f, 0.0f }, my_packet->rotAngle);
			auto p = players[id]->GetComponent<CharacterMovingBehavior>();
			p->move(my_packet->xPos, my_packet->zPos);
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

	case S2C_CONSTRUCT:
		break;
	case S2C_DESTRUCT:
		break;
	case S2C_DESTRUCT_ALL:
		break;
	case S2C_CHAT:
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
	auto retval = recv(serverSocket, net_buf, BUFSIZE, 0);

	if (retval > 0)	process_data(net_buf, retval);
}

void HostNetwork::send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);
	send(serverSocket, p, (unsigned char)p[0], 0);
}

void HostNetwork::send_construct_packet(int type, int name, float x, float z, float angle)
{
	cs_packet_construct m_packet;
	m_packet.type = C2S_CONSTRUCT;
	m_packet.size = sizeof(m_packet);
	m_packet.building_type = type;
	m_packet.building_name = name;
	m_packet.xpos = x;
	m_packet.zpos = z;
	m_packet.angle = angle;

	send_packet(&m_packet);
}

void HostNetwork::send_destruct_packet(int type, int name, float x, float z, float angle)
{
	cs_packet_destruct m_packet;
	m_packet.type = C2S_DESTRUCT;
	m_packet.size = sizeof(m_packet);
	//m_packet.building_type = type;
	//m_packet.building_name = name;
	//m_packet.xpos = x;
	//m_packet.zpos = z;
	//m_packet.angle = angle;

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
	cs_packet_login_host l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGIN_HOST;
	int t_id = GetCurrentProcessId();
	sprintf_s(l_packet.name, "P%03d", t_id % 1000);
	strcpy_s(name, l_packet.name);
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

	isConnect = false;
	tryConnect = false;

	closesocket(serverSocket);

	for (auto& p : players)
		Scene::scene->PushDelete(p.second);
	players.clear();
}
