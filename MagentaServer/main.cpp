#include "main.h"

class IOCPServer iocp;
class Timer	timer;
class Contents contents;

unordered_map<int, Client*> g_clients;
unordered_map<BuildingInform, class Building*, BuildingInformHasher> g_buildings;

mutex g_clients_lock;
//RWLock g_sector_clients_lock[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
mutex g_sector_clients_lock[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
unordered_set <class Client*> g_sector_clients[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
unordered_set <class Building*> g_sector_buildings[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];

class Terrain* terrain_data;

int main()
{
	iocp.init_server();
	iocp.start_server();
	contents.start_contents();
	timer.start_timer(iocp.get_iocp_handle());

	getchar();
	return 0;
}