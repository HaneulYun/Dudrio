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

extern class IOCPServer iocp;
extern class unordered_map<int, class Room*> g_rooms;
extern class unordered_map<int, class Client*> g_servers;
extern class unordered_map<int, class Client*> g_users;
