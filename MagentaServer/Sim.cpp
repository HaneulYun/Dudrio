#include "pch.h"
#include "Sim.h"

bool Sim::is_near(const Client& other)
{
	if (fabs(pos.x - other.m_xPos) > VIEW_RADIUS)	return false;
	if (fabs(pos.z - other.m_zPos) > VIEW_RADIUS)	return false;
	return true;
}

void Sim::Update()
{
	stateMachine.Update();
}

bool Sim::is_sector_change(float prevX, float prevZ)
{
	pair<int, int> prev_sect_num = contents.calculate_sector_num(prevX, prevZ);
	pair<int, int> now_sect_num = contents.calculate_sector_num(pos.x, pos.z);

	if (prev_sect_num == now_sect_num)
		return false;
	return true;
}

void Sim::erase_sim_in_sector(float x, float z)
{
	pair<int, int> sect_num = contents.calculate_sector_num(x, z);
	g_sector_sims_lock[sect_num.second][sect_num.first].lock();
	if (g_sector_sims[sect_num.second][sect_num.first].count(this) != 0)
		g_sector_sims[sect_num.second][sect_num.first].erase(this);
	g_sector_sims_lock[sect_num.second][sect_num.first].unlock();
}

void Sim::erase_sim_in_sector()
{
	pair<int, int> sect_num = contents.calculate_sector_num(pos.x, pos.z);
	g_sector_sims_lock[sect_num.second][sect_num.first].lock();
	if (g_sector_sims[sect_num.second][sect_num.first].count(this) != 0)
		g_sector_sims[sect_num.second][sect_num.first].erase(this);
	g_sector_sims_lock[sect_num.second][sect_num.first].unlock();
}

void Sim::insert_client_in_sector()
{
	pair<int, int> sect_num = contents.calculate_sector_num(pos.x, pos.z);
	g_sector_sims_lock[sect_num.second][sect_num.first].lock();
	g_sector_sims[sect_num.second][sect_num.first].insert(this);
	g_sector_sims_lock[sect_num.second][sect_num.first].unlock();
}

vector<int> Sim::get_near_clients()
{
	pair<int, int> sect_num = contents.calculate_sector_num(pos.x, pos.z);
	vector<int> near_clients;
	near_clients.clear();

	for (int i = sect_num.second - 1; i <= sect_num.second + 1; ++i) {
		if (i < 0 || i > WORLD_HEIGHT / SECTOR_WIDTH - 1) continue;
		for (int j = sect_num.first - 1; j <= sect_num.first + 1; ++j) {
			if (j < 0 || j > WORLD_WIDTH / SECTOR_WIDTH - 1) continue;
			lock_guard<mutex>lock_guard(g_sector_clients_lock[i][j]);
			for (auto nearObj : g_sector_clients[i][j]) {
				if (ST_ACTIVE != nearObj->m_status)	continue;
				if (true == is_near(*nearObj))
					near_clients.emplace_back(nearObj->m_id);
			}
		}
	}
	return near_clients;
}