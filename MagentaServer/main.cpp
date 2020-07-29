#include "pch.h"
#include "main.h"

class IOCPServer iocp;
class Timer	timer;
class Contents contents;

unordered_map <int, Client*> g_clients;
mutex g_clients_lock;
unordered_set <class Client*> g_sector_clients[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
mutex g_sector_clients_lock[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];

unordered_map <struct BuildingInfo, class Building*, struct BuildingInfoHasher> g_buildings[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
mutex g_buildings_lock;
unordered_set <class Village*> g_villages;

unordered_map <int, class Sim*> g_sims;
mutex g_sims_lock;
unordered_set <class Sim*> g_sector_sims[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];
mutex g_sector_sims_lock[WORLD_HEIGHT / SECTOR_WIDTH][WORLD_WIDTH / SECTOR_WIDTH];

class Terrain* terrain_data;

int main()
{
	iocp.init_server();
	iocp.start_server();
	contents.init_contents();
	timer.start_timer(iocp.get_iocp_handle());

	getchar();
	return 0;
}