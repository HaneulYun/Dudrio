#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WS2tcpip.h>
#include <MSWSock.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <windows.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <thread>
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

#include "math.h"
#include "protocol.h"
#include "Timer.h"
#include "IOCPServer.h"
#include "Objects.h"
#include "contents.h"

// AI --------------------------------
#include "AIManager.h"
#include "Sim.h"
#include "Village.h"
// Terrain
#include "TerrainGenerator.h"
#include "Terrain.h"
// Path
#include "PathFinder.h"
// FSM
#include "Message.h"
#include "MessageManager.h"
#include "Telegram.h"
#include "SimStates.h"
#include "State.h"
#include "StateMachine.h"
// -----------------------------------

extern class IOCPServer iocp;
extern class Timer	timer;
extern class Contents contents;

extern unordered_map<int, class Client*> g_clients;
extern mutex g_clients_lock;
extern unordered_set <class Client*> g_sector_clients[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
extern mutex g_sector_clients_lock[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];

extern unordered_map<struct BuildingInfo, class Building*, struct BuildingInfoHasher> g_buildings[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
extern mutex g_buildings_lock;

extern class Terrain* terrain_data;