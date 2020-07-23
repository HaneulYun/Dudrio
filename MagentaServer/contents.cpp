#include "contents.h"

Contents::Contents()
{
	logic_run = false;
	host_id = -1;
}

Contents::~Contents()
{
	stop_contents();
}

void Contents::init_contents()
{
	for (int i = 0; i < WORLD_HEIGHT / SECTOR_WIDTH; ++i)
		for (int j = 0; j < WORLD_WIDTH / SECTOR_WIDTH; ++j)
			g_sector_buildings[i][j].clear();

	for (auto& b : g_buildings)
		delete b.second;
	g_buildings.clear();

	if (terrain_data != nullptr)
		delete terrain_data;
}

void Contents::start_contents()
{
	logic_run = true;
	//logic_thread = thread([this]() { logic_thread_loop(); });
	logic_threads.reserve(NUM_OF_CPU * 2 + 2);
	
	for (int i = 0; i < NUM_OF_CPU * 2 + 1; ++i) {
		logic_threads.emplace_back([this]() {logic_thread_loop(); });
	}

	init_contents();
}

void Contents::stop_contents()
{
	logic_run = false;
	//if (logic_thread.joinable())
	//	logic_thread.join();
	for (auto& ths : logic_threads)
		if (ths.joinable())
			ths.join();
}

void Contents::logic_thread_loop()
{
	while (logic_run) {
		logic_lock.lock();
		if (!recvQueue.empty()) {
			//logic_lock.LeaveReadLock();
			//logic_lock.EnterWriteLock();
			auto buf = recvQueue.front();
			recvQueue.pop();
			//logic_lock.LeaveWriteLock();
			logic_lock.unlock();

			switch (buf.second[1]) {
			case C2S_LOGIN_GUEST: {
				if (host_id != -1) {
					cout << "The Guest " << buf.first << " is connected" << endl;
					cs_packet_login_guest* packet = reinterpret_cast<cs_packet_login_guest*>(buf.second);
					enter_game(buf.first, packet->name);
				}
				else {
					cout << "Host does not exist" << endl;
					login_fail(buf.first);
				}
			}
			break;
			case C2S_LOGIN_HOST:
			{
				if (host_id == -1) {
					cout << "The host " << buf.first << " is connected" << endl;
					cs_packet_login_host* packet = reinterpret_cast<cs_packet_login_host*>(buf.second);
					host_id = buf.first;

					TerrainGenerator terrainGenerator(packet->terrainSize, packet->terrainSize);
					string fileName = terrainGenerator.createHeightMap(packet->frequency, packet->octaves, packet->seed, (char*)"square");
					terrain_data = new Terrain;
					terrain_data->frequency = packet->frequency;
					terrain_data->terrain_size = packet->terrainSize;
					terrain_data->octaves = packet->octaves;
					terrain_data->seed = packet->seed;

					wstring name;
					name.assign(fileName.begin(), fileName.end());
					terrain_data->AlphamapTextureName = name.c_str();
					terrain_data->heightmapHeight = terrain_data->terrain_size;
					terrain_data->heightmapWidth = terrain_data->terrain_size;
					terrain_data->x_size = terrain_data->terrain_size;
					terrain_data->y_size = 255;
					terrain_data->z_size = terrain_data->terrain_size;
					terrain_data->Load();

					enter_game(buf.first, packet->name);
				}
				else {
					cout << "Host is already exist" << endl;
					login_fail(buf.first);
				}
			}
			break;
			case C2S_LOGOUT:
			{
				cs_packet_logout* packet = reinterpret_cast<cs_packet_logout*>(buf.second);
			}
			break;
			case C2S_MOVE_START:
			{
				cs_packet_move_start* packet = reinterpret_cast<cs_packet_move_start*>(buf.second);

				g_clients[buf.first]->m_last_move_time = GetTickCount64();
				g_clients[buf.first]->m_move_time = packet->move_time;
				do_move(buf.first, packet->xVel, packet->zVel, packet->rotAngle, packet->run_level);
			}
			break;
			case C2S_MOVE:
			{
				cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf.second);

				g_clients[buf.first]->m_move_time = packet->move_time;
				do_move(buf.first, packet->xVel, packet->zVel, packet->rotAngle, packet->run_level);
			}
			break;
			case C2S_CONSTRUCT:
			{
				cs_packet_construct* packet = reinterpret_cast<cs_packet_construct*>(buf.second);
				if (buf.first == host_id)
					do_construct(buf.first, packet->b_inform);
			}
			break;
			case C2S_DESTRUCT:
			{
				cs_packet_destruct* packet = reinterpret_cast<cs_packet_destruct*>(buf.second);
				if (buf.first == host_id)
					do_destruct(buf.first, packet->b_inform);
			}
			break;
			case C2S_DESTRUCT_ALL:
			{
				cs_packet_destruct_all* packet = reinterpret_cast<cs_packet_destruct_all*>(buf.second);
				if (buf.first == host_id)
					destruct_all(buf.first);
			}
			break;
			case C2S_CHAT:
			{

			}
			break;
			default:
				cout << "Unknown Packet Type Error!\n" << buf.first << ", " << buf.second << endl;
				//DebugBreak();
				//exit(-1);
				break;
			}
		}
		else
		//	//logic_lock.LeaveReadLock();
			logic_lock.unlock();
	}
}

void Contents::enter_game(int user_id, char name[])
{
	//g_clients[user_id]->m_cl.EnterWriteLock();
	g_clients[user_id]->m_cl.lock();
	strcpy_s(g_clients[user_id]->m_name, name);
	g_clients[user_id]->m_name[MAX_ID_LEN] = NULL;
	iocp.send_login_ok_packet(user_id);
	//g_clients[user_id]->m_cl.LeaveWriteLock();
	g_clients[user_id]->m_cl.unlock();
	g_clients[user_id]->m_status = ST_ACTIVE;

	if (host_id != user_id) {
		iocp.send_enter_packet(contents.host_id, user_id);

		g_clients[user_id]->insert_client_in_sector();
		vector<int> near_clients = g_clients[user_id]->get_near_clients();

		for (auto cl : near_clients) {
			if (ST_ACTIVE == g_clients[cl]->m_status) {
				iocp.send_enter_packet(user_id, cl);
				iocp.send_enter_packet(cl, user_id);
			}
		}

		for (auto b : g_buildings)
			iocp.send_construct_packet(user_id, b.first);
	}
	else {
		for (auto cl : g_clients) {
			if (cl.second->m_id == user_id) continue;
			if (ST_ACTIVE == cl.second->m_status) {
				iocp.send_enter_packet(user_id, cl.second->m_id);
				iocp.send_enter_packet(cl.second->m_id, user_id);
			}
		}
	}
}

void Contents::do_move(int user_id, float xVel, float zVel, float rotAngle, float run_level)
{
	// run_level == 2 : default
	float prev_x = g_clients[user_id]->m_xPos;
	float prev_z = g_clients[user_id]->m_zPos;

	g_clients[user_id]->m_rotAngle += rotAngle;

	float nomalize_vel = sqrt(pow(xVel, 2) + pow(zVel, 2));
	if (nomalize_vel != 0.0f) {
		g_clients[user_id]->m_xVel = xVel * run_level * 0.5 / nomalize_vel;
		g_clients[user_id]->m_zVel = zVel * run_level * 0.5 / nomalize_vel;
	}
	else {
		g_clients[user_id]->m_xVel = 0.0f;
		g_clients[user_id]->m_zVel = 0.0f;
	}
	g_clients[user_id]->m_xPos += g_clients[user_id]->m_xVel * (GetTickCount64() - g_clients[user_id]->m_last_move_time) / 1000.f;
	g_clients[user_id]->m_zPos += g_clients[user_id]->m_zVel * (GetTickCount64() - g_clients[user_id]->m_last_move_time) / 1000.f;
	g_clients[user_id]->m_last_move_time = GetTickCount64();

	if (g_clients[user_id]->m_xPos < 0.0f) g_clients[user_id]->m_xPos = 0.0f;
	if (g_clients[user_id]->m_zPos < 0.0f) g_clients[user_id]->m_zPos = 0.0f;
	if (g_clients[user_id]->m_xPos > 999.f) g_clients[user_id]->m_xPos = 999.f;
	if (g_clients[user_id]->m_zPos > 999.f) g_clients[user_id]->m_zPos = 999.f;

	if (true == g_clients[user_id]->is_sector_change(prev_x, prev_z)) {
		g_clients[user_id]->erase_client_in_sector(prev_x, prev_z);
		g_clients[user_id]->insert_client_in_sector();
	}

	//g_clients[user_id]->m_cl.EnterReadLock();
	g_clients[user_id]->m_cl.lock();
	unordered_set<int> old_vl = g_clients[user_id]->view_list;
	unordered_set<int> new_vl;
	//g_clients[user_id]->m_cl.LeaveReadLock();
	g_clients[user_id]->m_cl.unlock();

	vector<int> near_clients = g_clients[user_id]->get_near_clients();
	for (auto cl : near_clients) new_vl.insert(cl);

	if (contents.host_id == -1)
		return;
	iocp.send_move_packet(user_id, user_id, rotAngle);
	iocp.send_move_packet(contents.host_id, user_id, rotAngle);

	for (auto new_player : new_vl) {
		g_clients_lock.lock();
		if (g_clients.count(new_player) == 0){
			g_clients_lock.unlock();
			continue;
		}
		g_clients_lock.unlock();
		if (old_vl.count(new_player) == 0) {
			iocp.send_enter_packet(user_id, new_player);
			//g_clients[new_player]->m_cl.EnterReadLock();
			g_clients[new_player]->m_cl.lock();
			if (g_clients[new_player]->view_list.count(user_id) == 0) {
				//g_clients[new_player]->m_cl.LeaveReadLock();
				g_clients[new_player]->m_cl.unlock();
				iocp.send_enter_packet(new_player, user_id);
			}
			else {
				//g_clients[new_player]->m_cl.LeaveReadLock();
				g_clients[new_player]->m_cl.unlock();
				iocp.send_move_packet(new_player, user_id, rotAngle);
			}
		}
		else {
			//g_clients[new_player]->m_cl.EnterReadLock();
			g_clients[new_player]->m_cl.lock();
			if (0 != g_clients[new_player]->view_list.count(user_id)) {
				//g_clients[new_player]->m_cl.LeaveReadLock();
				g_clients[new_player]->m_cl.unlock();
				iocp.send_move_packet(new_player, user_id, rotAngle);
			}
			else {
				//g_clients[new_player]->m_cl.LeaveReadLock();
				g_clients[new_player]->m_cl.unlock();
				iocp.send_enter_packet(new_player, user_id);
			}
		}
	}

	for (auto old_player : old_vl) {
		g_clients_lock.lock();
		if (g_clients.count(old_player) == 0) {
			g_clients_lock.unlock();
			continue;
		}
		g_clients_lock.unlock();
		if (new_vl.count(old_player) == 0) {
			iocp.send_leave_packet(user_id, old_player);
			//g_clients[old_player]->m_cl.EnterReadLock();
			g_clients[old_player]->m_cl.lock();
			if (0 != g_clients[old_player]->view_list.count(user_id)) {
				//g_clients[old_player]->m_cl.LeaveReadLock();
				g_clients[old_player]->m_cl.unlock();
				iocp.send_leave_packet(old_player, user_id);
			}
			else {
				g_clients[old_player]->m_cl.unlock();
				//g_clients[old_player]->m_cl.LeaveReadLock();
			}
		}
	}
}

void Contents::disconnect(int user_id)
{
	lock_guard<mutex>lock_guard(g_clients_lock);
	cout << "Disconnect " << user_id << endl;

	iocp.send_leave_packet(user_id, user_id);

	if (host_id != user_id)
		g_clients[user_id]->erase_client_in_sector();
	g_clients[user_id]->m_cl.lock();
	g_clients[user_id]->m_status = ST_ALLOC;

	linger sopt_linger = { 0, 0 };	
	shutdown(g_clients[user_id]->m_s, SD_BOTH);
	setsockopt(g_clients[user_id]->m_s, SOL_SOCKET, SO_LINGER, (char*)&sopt_linger, sizeof(sopt_linger));
	closesocket(g_clients[user_id]->m_s);
	g_clients[user_id]->m_s = INVALID_SOCKET;

	if (host_id != user_id){
		//g_clients[user_id]->m_cl.lock();
		for (auto cl = g_clients[user_id]->view_list.begin(); cl != g_clients[user_id]->view_list.end(); ++cl) {
			if (user_id == *cl) continue;
			if (ST_ACTIVE == g_clients[*cl]->m_status)
				iocp.send_leave_packet(*cl, user_id);
		}
		//g_clients[user_id]->m_cl.unlock();
		if (host_id != -1)
			iocp.send_leave_packet(host_id, user_id);
	}
	else {
		for (auto cl = g_clients.begin(); cl != g_clients.end();) {
			if (cl->first == user_id) { ++cl; continue; }
			if (ST_ACTIVE == cl->second->m_status) {
				cout << "Disconnect " << cl->second->m_id << endl;
				iocp.send_leave_packet(cl->first, cl->first);
				delete cl->second;
				cl = g_clients.erase(cl);
			}
			else
				++cl;
		}

		host_id = -1;

		init_contents();

		cout << "Disconnect the host" << endl;
	}
	g_clients[user_id]->m_cl.unlock();

	delete g_clients[user_id];
	g_clients.erase(user_id);
}

void Contents::login_fail(int user_id)
{
	cout << "Login fail!! user_id : " << user_id << endl;
	lock_guard<mutex>lock_guard(g_clients_lock);

	if (g_clients.count(user_id) == 0)
		return;
	if (host_id != user_id)
		g_clients[user_id]->erase_client_in_sector();
	iocp.send_login_fail_packet(user_id);
	g_clients[user_id]->m_status = ST_ALLOC;
	linger sopt_linger = { 0, 0 };
	shutdown(g_clients[user_id]->m_s, SD_BOTH);
	setsockopt(g_clients[user_id]->m_s, SOL_SOCKET, SO_LINGER, (char*)&sopt_linger, sizeof(sopt_linger));
	closesocket(g_clients[user_id]->m_s);
	g_clients[user_id]->m_s = INVALID_SOCKET;

	delete g_clients[user_id];
	g_clients.erase(user_id);
}

void Contents::do_construct(int user_id, BuildingInform b_inform)
{
	g_buildings[b_inform] = new Building(b_inform);
	pair<int, int> b_sectnum = calculate_sector_num(b_inform.xPos, b_inform.zPos);
	g_sector_buildings[b_sectnum.second][b_sectnum.first].insert(g_buildings[b_inform]);
	
	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_construct_packet(cl.second->m_id, b_inform);
	}
}

void Contents::do_destruct(int user_id, BuildingInform b_inform)
{
	pair<int, int> b_sectnum = calculate_sector_num(b_inform.xPos, b_inform.zPos);
	g_sector_buildings[b_sectnum.second][b_sectnum.first].erase(g_buildings[b_inform]);
	
	delete g_buildings[b_inform];
	g_buildings.erase(b_inform);

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_destruct_packet(cl.second->m_id, b_inform);
	}
}

void Contents::destruct_all(int user_id)
{
	init_contents();

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_destruct_all_packet(cl.second->m_id);
	}
}

void Contents::add_packet(int user_id, char* buf)
{
	//logic_lock.EnterWriteLock();
	logic_lock.lock();
	recvQueue.push(make_pair(user_id, buf));
	//logic_lock.LeaveWriteLock();
	logic_lock.unlock();
}

pair<int, int> Contents::calculate_sector_num(float xPos, float zPos)
{
	int x_sectnum = floor(xPos / SECTOR_WIDTH);
	int z_sectnum = floor(zPos / SECTOR_WIDTH);
	
	return make_pair(x_sectnum, z_sectnum);
}