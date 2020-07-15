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
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <queue>
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

#include "RWLock.h"
#include "protocol.h"
#include "TerrainGenerator.h"
#include "Timer.h"
#include "IOCPServer.h"
#include "Clients.h"
#include "contents.h"

extern class IOCPServer iocp;
extern class Timer	timer;
extern class Contents contents;

extern unordered_map<int, class Clients*> g_clients;
extern vector<BuildingInform> buildings;
