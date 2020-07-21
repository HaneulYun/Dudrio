#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WS2tcpip.h>
#include <MSWSock.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <windows.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <random>
#include <string>

using namespace std;
using namespace chrono;

constexpr auto MAX_PACKET_SIZE = 255;
constexpr auto MAX_BUF_SIZE = 1024;

enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT };
enum C_STATUS { ST_FREE, ST_ALLOC, ST_ACTIVE };

struct EXOVER {
	WSAOVERLAPPED	over;
	ENUMOP			op;
	char			io_buf[MAX_BUF_SIZE];

	union {
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
};

#define SECTOR_WIDTH 20
#define VIEW_RADIUS	 20

#include "RWLock.h"
#include "protocol.h"
#include "TerrainGenerator.h"
#include "Timer.h"
#include "IOCPServer.h"
#include "Objects.h"
#include "contents.h"

extern class IOCPServer iocp;
extern class Timer	timer;
extern class Contents contents;

extern unordered_map<int, class Client*> g_clients;
extern unordered_map<BuildingInform, class Building*, BuildingInformHasher> g_buildings;

//extern RWLock g_sector_clients_lock[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
extern mutex g_sector_clients_lock[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
extern unordered_set <class Client*> g_sector_clients[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
extern unordered_set <class Building*> g_sector_buildings[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];

extern class Terrain* terrain_data;