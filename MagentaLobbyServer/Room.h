#pragma once
#include "Clients.h"

class Room {
public:
	int room_id;
	int port_num;
	std::string serverIP;

	// 마을 지형 정보
	int terrain_size;
	int frequency;
	int octaves;
	int seed;

	// 호스트 정보
	Client* host;

	// 접속한 게스트들 정보
	vector<Client*> guests;

public:
	Room() {}
	~Room() {}
};