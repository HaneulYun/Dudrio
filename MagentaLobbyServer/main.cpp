#include "pch.h"
#include "main.h"

class IOCPServer iocp;
unordered_map<int, class Room*> g_rooms;
unordered_map<int, class Client*> g_servers;
unordered_map<int, class Client*> g_users;

int main()
{
	iocp.init_server();
	iocp.start_server();

	getchar();
	return 0;
}