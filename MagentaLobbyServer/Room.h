#pragma once
#include "Clients.h"

class Room {
public:
	int room_id;
	int port_num;
	char serverIP[INET_ADDRSTRLEN];

	// 마을 지형 정보
	int terrain_size;
	int frequency;
	int octaves;
	int seed;

	// 호스트 이름
	bool is_host_exist;
	char host_name[L_MAX_ID_LEN + 1];

public:
	Room() {}
	Room(int id, char* ip) : room_id(id) { 
		strcpy_s(serverIP, ip);
		port_num = 9000 + ((id + 3) * 4); 
		is_host_exist = false;
	}
	~Room() {}

	void make_empty()
	{
		terrain_size = 0;
		frequency = 0;
		octaves = 0;
		seed = 0;
		is_host_exist = false;
		host_name[0] = '\0';
	}
};