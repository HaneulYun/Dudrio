#pragma once
#include "main.h"

struct TargetInfo
{
	Vector2D pos;
	float posOffset = 0;

	TargetInfo(Vector2D pos, float posOffset) : pos(pos), posOffset(posOffset) {}
	TargetInfo() = default;
};

class Sim
{
public:
	int id;

	Vector2D pos;
	Vector2D forward;
	float rotAngle;

	Building* home;

	StateMachine<Sim> stateMachine;
	deque<TargetInfo> targetPos;
	deque<Vector2D> path;
	BuildMessageInfo buildInfo;
	vector<int>	prevNearClients;
public:
	Sim() :stateMachine(this) {}
	Sim(int idx, float x, float z) :id(idx), pos(x, z), forward(0, 1), rotAngle(0), stateMachine(this) {}
	~Sim() {}

	bool is_near(const Client& other)
	{
		if (fabs(pos.x - other.m_xPos) > VIEW_RADIUS)	return false;
		if (fabs(pos.z - other.m_zPos) > VIEW_RADIUS)	return false;
		return true;
	}

	void Update()
	{
		stateMachine.Update();
	}

	bool is_sector_change(float prevX, float prevZ)
	{
		pair<int, int> prev_sect_num = contents.calculate_sector_num(prevX, prevZ);
		pair<int, int> now_sect_num = contents.calculate_sector_num(pos.x, pos.z);

		if (prev_sect_num == now_sect_num)
			return false;
		return true;
	}

	void erase_sim_in_sector(float x, float z)
	{
		pair<int, int> sect_num = contents.calculate_sector_num(x, z);
		g_sector_sims_lock[sect_num.second][sect_num.first].lock();
		if (g_sector_sims[sect_num.second][sect_num.first].count(this) != 0)
			g_sector_sims[sect_num.second][sect_num.first].erase(this);
		g_sector_sims_lock[sect_num.second][sect_num.first].unlock();
	}

	void erase_sim_in_sector()
	{
		pair<int, int> sect_num = contents.calculate_sector_num(pos.x, pos.z);
		g_sector_sims_lock[sect_num.second][sect_num.first].lock();
		if (g_sector_sims[sect_num.second][sect_num.first].count(this) != 0)
			g_sector_sims[sect_num.second][sect_num.first].erase(this);
		g_sector_sims_lock[sect_num.second][sect_num.first].unlock();
	}

	void insert_client_in_sector()
	{
		pair<int, int> sect_num = contents.calculate_sector_num(pos.x, pos.z);
		g_sector_sims_lock[sect_num.second][sect_num.first].lock();
		g_sector_sims[sect_num.second][sect_num.first].insert(this);
		g_sector_sims_lock[sect_num.second][sect_num.first].unlock();
	}

	vector<int> get_near_clients()
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

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};