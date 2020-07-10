#pragma once
#pragma comment(lib, "ws2_32.lib");

#define _WINSOCK_DEPRECATED_NO_WARNINGS;

#include <WS2tcpip.h>
#include <MSWSock.h>
#include <iostream>
#include <thread>
#include <windows.h>
#include <unordered_map>
#include <chrono>
#include <queue>
#include <mutex>

using namespace std;
using namespace chrono;

#include "RWLock.h"
#include "protocol.h"
#include "Timer.h"
#include "IOCPServer.h"
#include "Clients.h"
#include "contents.h"

extern class IOCPServer iocp;
extern class Timer	timer;
extern class Contents contents;
