#include "Objects.h"

Client::Client(SOCKET& sock, int id)
{
	m_id = id;
	m_prev_size = 0;
	m_recv_over.op = OP_RECV;
	ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
	m_recv_over.wsabuf.buf = m_recv_over.io_buf;
	m_recv_over.wsabuf.len = MAX_BUF_SIZE;
	m_status = ST_FREE;
	m_s = sock;
	m_xPos = 540.0;	m_yPos = 0.0; m_zPos = 540.0;
	m_xVel = 0.0;	m_zVel = 0.0;
	m_rotAngle = 0.0f;
}

Client::Client(int id)
{
	m_id = id;
	m_prev_size = 0;
	m_recv_over.op = OP_RECV;
	ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
	m_recv_over.wsabuf.buf = m_recv_over.io_buf;
	m_recv_over.wsabuf.len = MAX_BUF_SIZE;
	m_status = ST_FREE;
	m_xPos = 540.0;	m_yPos = 0.0; m_zPos = 540.0;
	m_xVel = 0.0;	m_zVel = 0.0;
	m_rotAngle = 0.0f;
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
	g_sector_clients_lock[sect_num.second][sect_num.first].EnterWriteLock();
	if (g_sector_clients[sect_num.second][sect_num.first].count(this) != 0)
		g_sector_clients[sect_num.second][sect_num.first].erase(this);
	g_sector_clients_lock[sect_num.second][sect_num.first].LeaveWriteLock();
}

void Client::erase_client_in_sector()
{
	pair<int, int> sect_num = contents.calculate_sector_num(m_xPos, m_zPos);
	g_sector_clients_lock[sect_num.second][sect_num.first].EnterWriteLock();
	if (g_sector_clients[sect_num.second][sect_num.first].count(this) != 0)
		g_sector_clients[sect_num.second][sect_num.first].erase(this);
	g_sector_clients_lock[sect_num.second][sect_num.first].LeaveWriteLock();
}

void Client::insert_client_in_sector()
{
	pair<int, int> sect_num = contents.calculate_sector_num(m_xPos, m_zPos);
	g_sector_clients_lock[sect_num.second][sect_num.first].EnterWriteLock();
	g_sector_clients[sect_num.second][sect_num.first].insert(this);
	g_sector_clients_lock[sect_num.second][sect_num.first].LeaveWriteLock();
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
			g_sector_clients_lock[i][j].EnterReadLock();
			for (auto nearObj : g_sector_clients[i][j]) {
				if (nearObj->m_id == m_id) continue;
				if (true == is_near(*nearObj))
					near_clients.emplace_back(nearObj->m_id);
			}
			g_sector_clients_lock[i][j].LeaveReadLock();
		}
	}
	return near_clients;
}

void Client::enter_game(char name[])
{
	strcpy_s(m_name, name);
	m_name[MAX_ID_LEN] = NULL;
	iocp.send_login_ok_packet(m_id);
	m_status = ST_ACTIVE;

	if (!is_host) {
		iocp.send_enter_packet(contents.host_id, m_id);

		insert_client_in_sector();
		vector<int> near_clients = get_near_clients();

		for (auto cl : near_clients) {
			if (ST_ACTIVE == g_clients[cl]->m_status) {
				iocp.send_enter_packet(m_id, cl);
				iocp.send_enter_packet(cl, m_id);
			}
		}

		for (auto b : g_buildings)
			iocp.send_construct_packet(m_id, b.first);
	}
	else {
		for (auto cl : g_clients) {
			if (cl.second->m_id == m_id) continue;
			if (ST_ACTIVE == cl.second->m_status) {
				iocp.send_enter_packet(m_id, cl.second->m_id);
				iocp.send_enter_packet(cl.second->m_id, m_id);
			}
		}
	}
}

void Client::do_move(float xVel, float zVel, float rotAngle, float run_level)
{
	// run_level == 2 : default
	float prev_x = m_xPos;
	float prev_z = m_zPos;

	m_rotAngle += rotAngle;

	float nomalize_vel = sqrt(pow(xVel, 2) + pow(zVel, 2));
	if (nomalize_vel != 0.0f) {
		m_xVel = xVel * run_level * 0.5 / nomalize_vel;
		m_zVel = zVel * run_level * 0.5 / nomalize_vel;
	}
	else {
		m_xVel = 0.0f;
		m_zVel = 0.0f;
	}
	m_xPos += m_xVel * (GetTickCount64() - m_last_move_time) / 1000.f;
	m_zPos += m_zVel * (GetTickCount64() - m_last_move_time) / 1000.f;
	m_last_move_time = GetTickCount64();

	if (is_sector_change(prev_x, prev_z)) {
		erase_client_in_sector(prev_x, prev_z);
		insert_client_in_sector();
	}

	unordered_set<int> old_vl = view_list;
	unordered_set<int> new_vl;

	vector<int> near_clients = get_near_clients();
	for (auto cl : near_clients) new_vl.insert(cl);

	iocp.send_move_packet(m_id, m_id, rotAngle);
	iocp.send_move_packet(contents.host_id, m_id, rotAngle);

	for (auto new_player : new_vl) {
		if (old_vl.count(new_player) == 0) {
			iocp.send_enter_packet(m_id, new_player);
			g_clients[new_player]->m_cl.EnterReadLock();
			if (g_clients[new_player]->view_list.count(m_id) == 0) {
				g_clients[new_player]->m_cl.LeaveReadLock();
				iocp.send_enter_packet(new_player, m_id);
			}
			else {
				g_clients[new_player]->m_cl.LeaveReadLock();
				iocp.send_move_packet(new_player, m_id, rotAngle);
			}
		}
		else {
			g_clients[new_player]->m_cl.EnterReadLock();
			if (0 != g_clients[new_player]->view_list.count(m_id)) {
				g_clients[new_player]->m_cl.LeaveReadLock();
				iocp.send_move_packet(new_player, m_id, rotAngle);
			}
			else {
				g_clients[new_player]->m_cl.LeaveReadLock();
				iocp.send_enter_packet(new_player, m_id);
			}
		}
	}

	for (auto old_player : old_vl) {		
		if (new_vl.count(old_player) == 0) {
			iocp.send_leave_packet(m_id, old_player);
			g_clients[old_player]->m_cl.EnterReadLock();
			if (0 != g_clients[old_player]->view_list.count(m_id)) {
				g_clients[old_player]->m_cl.LeaveReadLock();
				iocp.send_leave_packet(old_player, m_id);
			}
			else {
				g_clients[old_player]->m_cl.LeaveReadLock();
			}
		}
	}
}