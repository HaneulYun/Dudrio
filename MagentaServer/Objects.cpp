#include "Objects.h"

Client::Client(SOCKET& sock, int id)
{
	m_cl.lock();
	m_id = id;
	m_prev_size = 0;
	m_recv_over.op = OP_RECV;
	ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
	m_recv_over.wsabuf.buf = m_recv_over.io_buf;
	m_recv_over.wsabuf.len = MAX_BUF_SIZE;
	m_status = ST_FREE;
	m_s = sock;
	// StressTest ----------------------------------------
	random_device rd;
	default_random_engine dre;
	uniform_real_distribution<>urd(0.0f, 1000.0f);
	m_xPos = urd(dre);	m_yPos = 0.0; m_zPos = urd(dre);
	// ---------------------------------------------------
	//m_xPos = 500.0;	m_yPos = 0.0; m_zPos = 500.0;
	// ---------------------------------------------------
	m_xVel = 0.0;	m_zVel = 0.0;
	m_rotAngle = 0.0f;
	m_cl.unlock();
}

Client::Client(int id)
{
	m_cl.lock();
	m_id = id;
	m_prev_size = 0;
	m_recv_over.op = OP_RECV;
	ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
	m_recv_over.wsabuf.buf = m_recv_over.io_buf;
	m_recv_over.wsabuf.len = MAX_BUF_SIZE;
	m_status = ST_FREE;
	// StressTest ----------------------------------------
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<>urd(0.0f, 1000.0f);
	m_xPos = urd(dre);	m_yPos = 0.0; m_zPos = urd(dre);
	// ---------------------------------------------------
	m_xPos = 500.0;	m_yPos = 0.0; m_zPos = 500.0;
	// ---------------------------------------------------
	m_xVel = 0.0;	m_zVel = 0.0;
	m_rotAngle = 0.0f;
	m_cl.unlock();
}

Client::~Client()
{

}

bool Client::is_sector_change(float prevX, float prevZ)
{
	pair<int, int> prev_sect_num = contents.calculate_sector_num(prevX, prevZ);
	pair<int, int> now_sect_num = contents.calculate_sector_num(m_xPos, m_zPos);

	if (prev_sect_num == now_sect_num)
		return false;
	return true;
}

void Client::erase_client_in_sector(float x, float z)
{
	pair<int, int> sect_num = contents.calculate_sector_num(x, z);
	g_sector_clients_lock[sect_num.second][sect_num.first].lock();
	if (g_sector_clients[sect_num.second][sect_num.first].count(this) != 0)
		g_sector_clients[sect_num.second][sect_num.first].erase(this);
	g_sector_clients_lock[sect_num.second][sect_num.first].unlock();
}

void Client::erase_client_in_sector()
{
	pair<int, int> sect_num = contents.calculate_sector_num(m_xPos, m_zPos);
	g_sector_clients_lock[sect_num.second][sect_num.first].lock();
	if (g_sector_clients[sect_num.second][sect_num.first].count(this) != 0)
		g_sector_clients[sect_num.second][sect_num.first].erase(this);
	g_sector_clients_lock[sect_num.second][sect_num.first].unlock();
}

void Client::insert_client_in_sector()
{
	pair<int, int> sect_num = contents.calculate_sector_num(m_xPos, m_zPos);
	g_sector_clients_lock[sect_num.second][sect_num.first].lock();
	g_sector_clients[sect_num.second][sect_num.first].insert(this);
	g_sector_clients_lock[sect_num.second][sect_num.first].unlock();
}

vector<int> Client::get_near_clients()
{
	pair<int, int> sect_num = contents.calculate_sector_num(m_xPos, m_zPos);
	vector<int> near_clients;
	near_clients.clear();

	for (int i = sect_num.second - 1; i <= sect_num.second + 1; ++i) {
		if (i < 0 || i > WORLD_HEIGHT / SECTOR_WIDTH - 1) continue;
		for (int j = sect_num.first - 1; j <= sect_num.first + 1; ++j) {
			if (j < 0 || j > WORLD_WIDTH / SECTOR_WIDTH - 1) continue;
			lock_guard<mutex>lock_guard(g_sector_clients_lock[i][j]);
			for (auto nearObj : g_sector_clients[i][j]) {
				if (ST_ACTIVE != nearObj->m_status)	continue;
				if (nearObj->m_id == m_id) continue;
				if (true == is_near(*nearObj))
					near_clients.emplace_back(nearObj->m_id);
			}
		}
	}
	return near_clients;
}

vector<pair<BuildingInfo, pair<int, int>>> Client::get_near_buildings(float x, float z)
{
	pair<int, int> sect_num = contents.calculate_sector_num(x, z);
	vector<pair<BuildingInfo, pair<int, int>>> near_buildings;
	near_buildings.clear();

	lock_guard<mutex>lock_guard(g_buildings_lock);
	for (int i = sect_num.second - 1; i <= sect_num.second + 1; ++i) {
		if (i < 0 || i > WORLD_HEIGHT / SECTOR_WIDTH - 1) continue;
		for (int j = sect_num.first - 1; j <= sect_num.first + 1; ++j) {
			if (j < 0 || j > WORLD_WIDTH / SECTOR_WIDTH - 1) continue;
			for (auto nearObj : g_buildings[i][j]) {
				if(true == nearObj.first.is_near(x, z))
					near_buildings.emplace_back(make_pair(nearObj.first, make_pair(i, j)));
			}
		}
	}
	return near_buildings;
}

void Client::is_collide(float prevX, float prevZ)
{
	vector<pair<BuildingInfo, pair<int, int>>> near_buildings = get_near_buildings(m_xPos, m_zPos);
	float min_dist = 100.f;
	for (auto b : near_buildings){
		if (g_buildings[b.second.first][b.second.second][b.first]->is_collide(m_xPos, m_zPos, m_rotAngle))
		{
			m_xPos = prevX;
			m_zPos = prevZ;
			return;
		}
	}
}

bool Building::is_collide(float player_x, float player_z, float player_angle)
{
	Vector2D dist_vector = getDistanceVector(m_info.m_xPos, m_info.m_zPos, player_x, player_z);
	Vector2D vectors[4];
	vectors[0] = getHeightVector(m_collider.m_z1, m_collider.m_z2, m_info.m_angle);
	vectors[1] = getHeightVector(player_z - 0.25, player_z + 0.25, player_angle);
	vectors[2] = getWidthVector(m_collider.m_x1, m_collider.m_x2, m_info.m_angle);
	vectors[3] = getWidthVector(player_x - 0.25, player_x + 0.25, player_angle);

	for (int i = 0; i < 4; ++i) {
		double sum = 0;
		Vector2D unit_vector = vectors[i].Normalize();
		for (int j = 0; j < 4; ++j)
			sum += dotproduct(vectors[j], unit_vector);
		if (dotproduct(dist_vector, unit_vector) > sum)
			return false;
	}

	return true;
}