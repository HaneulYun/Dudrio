#pragma once
#include "Clients.h"

class Room {
public:
	int room_id;
	int port_num;
	std::string serverIP;

	// ���� ���� ����
	int terrain_size;
	int frequency;
	int octaves;
	int seed;

	// ȣ��Ʈ ����
	Client* host;

	// ������ �Խ�Ʈ�� ����
	vector<Client*> guests;

public:
	Room() {}
	~Room() {}
};