#include "pch.h"
#include "contents.h"

Contents::Contents()
{
	host_id = -1;
}

Contents::~Contents()
{
}

void Contents::init_contents()
{
	timer_event ev{ 0, GAME_Reset, high_resolution_clock::now(),0, NULL };
	timer.add_event(ev);

	host_id = -1;
	tick_count = 0.f;
	ingame_time = 0.f;

	sleep_flag = false;
	wakeup_flag = false;
	for (int i = 0; i < 4; ++i)
		send_packet_flag[i] = false;

	init_sector();
	init_buildings();
	init_sims();
	init_colliders_inform();

	if (terrain_data != nullptr)
		delete terrain_data;
}

void Contents::init_sector()
{
	for (int i = 0; i < WORLD_HEIGHT / SECTOR_WIDTH; ++i)
		for (int j = 0; j < WORLD_WIDTH / SECTOR_WIDTH; ++j) {
			lock_guard<mutex>lock_guard(g_sector_clients_lock[i][j]);
			g_sector_clients[i][j].clear();
		}
}

void Contents::init_buildings()
{
	g_villages.clear();

	lock_guard<mutex>lock_guard(g_buildings_lock);
	for (int i = 0; i < WORLD_HEIGHT / SECTOR_WIDTH; ++i)
		for (int j = 0; j < WORLD_WIDTH / SECTOR_WIDTH; ++j) {
			for (auto b : g_buildings[i][j])
				delete b.second;
			g_buildings[i][j].clear();
			g_buildings[i][j].reserve(500);
		}
}

void Contents::init_sims()
{
	sim_index = 0;

	for (int i = 0; i < WORLD_HEIGHT / SECTOR_WIDTH; ++i)
		for (int j = 0; j < WORLD_WIDTH / SECTOR_WIDTH; ++j) {
			lock_guard<mutex>lock_guard(g_sector_sims_lock[i][j]);
			g_sector_sims[i][j].clear();
		}

	lock_guard<mutex>lock_guard(g_sims_lock);
	for (auto& sims : g_sims) 
		delete sims.second;
	g_sims.clear();
}

void Contents::init_colliders_inform()
{
	ifstream in("colliders.txt");
	istream_iterator<float> p(in);
	while (p != istream_iterator<float>()) {
		int type = *p++; int name = *p++;
		float x1 = *p++, z1 = *p++, x2 = *p++, z2 = *p++;
		if (type == 1 && name == 1) {
			x1 = collider_info[1][0].m_x1;
			z1 = collider_info[1][0].m_z1;
			x2 = collider_info[1][0].m_x2;
			z2 = collider_info[1][0].m_z2;
		}
		collider_info[type].push_back({ x1, z1, x2, z2 });
		cout << type << " " << name << " " << x1 << " " << z1 << " " << x2 << " " << z2<< endl;
	}
	in.close();
	for (auto info : collider_info[Landscape]) {
		collider_info[Nature].push_back(info);
	}
}

void Contents::process_packet(int user_id, char* buf)
{
	switch (buf[1]) {
	case C2S_LOGIN_GUEST: {
		if (host_id != -1) {
			cout << "The Guest " << user_id << " is connected" << endl;
			cs_packet_login_guest* packet = reinterpret_cast<cs_packet_login_guest*>(buf);
			enter_game(user_id, packet->name, packet->appearance);
		}
		else {
			cout << "Host does not exist" << endl;
			login_fail(user_id);
		}
	}
	break;
	case C2S_LOGIN_HOST:
	{
		if (host_id == -1) {
			cout << "The host " << user_id << " is connected" << endl;
			cs_packet_login_host* packet = reinterpret_cast<cs_packet_login_host*>(buf);
			host_id = user_id;

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
			terrain_data->size = Vector3D(terrain_data->terrain_size, 255, terrain_data->terrain_size);
			terrain_data->Load();
			terrain_data->makeExtraData();
			PathFinder::Instance()->SetTerrainData(terrain_data);

			server_time = GetTickCount64();
			ingame_time = packet->game_time;
			update();
			enter_game(user_id, packet->name, 0);
		}
		else {
			cout << "Host is already exist" << endl;
			login_fail(user_id);
		}
	}
	break;
	case C2S_LOGOUT:
	{
		cs_packet_logout* packet = reinterpret_cast<cs_packet_logout*>(buf);
	}
	break;
	case C2S_MOVE_START:
	{
		cs_packet_move_start* packet = reinterpret_cast<cs_packet_move_start*>(buf);

		g_clients[user_id]->m_last_move_time = GetTickCount64();
		g_clients[user_id]->m_move_time = packet->move_time;
		do_move(user_id, packet->xVel, packet->zVel, packet->rotAngle, packet->run_level);
	}
	break;
	case C2S_MOVE:
	{
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);

		g_clients[user_id]->m_move_time = packet->move_time;
		do_move(user_id, packet->xVel, packet->zVel, packet->rotAngle, packet->run_level);
	}
	break;
	case C2S_CONSTRUCT:
	{
		cs_packet_construct* packet = reinterpret_cast<cs_packet_construct*>(buf);
		if (user_id == host_id)
			do_construct(user_id, packet->building_type, packet->building_name, packet->xpos, packet->zpos, packet->angle, packet->landmark_range, packet->develop);
	}
	break;
	case C2S_DESTRUCT:
	{
		cs_packet_destruct* packet = reinterpret_cast<cs_packet_destruct*>(buf);
		if (user_id == host_id)
			do_destruct(user_id, packet->building_type, packet->building_name, packet->xPos, packet->zPos, packet->angle);
	}
	break;
	case C2S_DESTRUCT_ALL:
	{
		cs_packet_destruct_all* packet = reinterpret_cast<cs_packet_destruct_all*>(buf);
		if (user_id == host_id)
			destruct_all(user_id);
	}
	break;
	case C2S_LANDMARK_CHANGE:
	{
		cs_packet_landmark_change* packet = reinterpret_cast<cs_packet_landmark_change*>(buf);
		if (user_id == host_id) {
			lock_guard<mutex> lock_guard(g_buildings_lock);
			for (auto& landmark : g_villages) {
				if (landmark->m_info.m_xPos == packet->xpos && landmark->m_info.m_zPos == packet->zpos)	{
					landmark->autoDevelopment = packet->development;
					break;
				}
			}
		}
	}
	break;
	case C2S_CHAT:
	{
		cs_packet_chat* packet = reinterpret_cast<cs_packet_chat*>(buf);
		chatting(user_id, packet->message);
	}
	break;
	case C2S_TELEPORT:
	{
		cs_packet_teleport* packet = reinterpret_cast<cs_packet_teleport*>(buf);
		teleport(user_id, packet->xPos, packet->zPos);
	}
	break;
	default:
		cout << "Unknown Packet Type Error!\n" << user_id << ", " << buf << endl;
		//DebugBreak();
		//exit(-1);
		break;
	}
}


void Contents::enter_game(int user_id, wchar_t name[], char appearance)
{
	g_clients[user_id]->m_cl.lock();
	g_clients[user_id]->m_name[0] = '\0';
	wcscpy_s(g_clients[user_id]->m_name, name);
	g_clients[user_id]->m_name[MAX_ID_LEN] = NULL;
	g_clients[user_id]->m_appearance = appearance;

	if (host_id != user_id) {
		g_buildings_lock.lock();
		if (!g_villages.empty()) {
			auto begin = g_villages.begin();
			int size = rand() % g_villages.size();
			while (size > 0) {
				begin++; size--;
			}
			Village* village = *begin;
			g_clients[user_id]->m_xPos = village->m_info.m_xPos - (village->m_land_range / 2) + (rand() % village->m_land_range);
			g_clients[user_id]->m_zPos = village->m_info.m_zPos - (village->m_land_range / 2) + (rand() % village->m_land_range);
		}
		g_buildings_lock.unlock();
	}
	iocp.send_login_ok_packet(user_id);
	g_clients[user_id]->m_status = ST_ACTIVE;
	g_clients[user_id]->m_cl.unlock();

	if (host_id != user_id) {	
		iocp.send_enter_packet(contents.host_id, user_id);

		g_clients[user_id]->m_cl.lock();
		g_clients[user_id]->insert_client_in_sector();
		vector<int> near_clients = g_clients[user_id]->get_near_clients();
		vector<pair<BuildingInfo, pair<int, int>>> near_buildings = g_clients[user_id]->get_near_buildings();
		g_buildings_lock.lock();
		for (auto cl : near_buildings)
			if (g_buildings[cl.second.first][cl.second.second][cl.first]->is_collide(g_clients[user_id]->m_xPos, g_clients[user_id]->m_zPos, g_clients[user_id]->m_rotAngle)) {
				g_clients[user_id]->m_collide_invincible = true;
				break;
			}
		g_buildings_lock.unlock();
		g_clients[user_id]->m_cl.unlock();

		for (auto cl : near_clients) {
			if (ST_ACTIVE == g_clients[cl]->m_status) {
				iocp.send_enter_packet(user_id, cl);
				iocp.send_enter_packet(cl, user_id);
			}
		}

		g_buildings_lock.lock();
		for (auto& landmark : g_villages) 
			iocp.send_construct_packet(user_id, landmark->m_info.building_type, landmark->m_info.building_name, landmark->m_info.m_xPos, landmark->m_info.m_zPos, landmark->m_info.m_angle, landmark->m_land_range);

		for (int i = 0; i < WORLD_HEIGHT / SECTOR_WIDTH; ++i)
			for (int j = 0; j < WORLD_WIDTH / SECTOR_WIDTH; ++j)
				for (auto b : g_buildings[i][j]) {
					auto p = dynamic_cast<Village*>(b.second);
					if (p == nullptr)
						iocp.send_construct_packet(user_id, b.second->m_info.building_type, b.second->m_info.building_name, b.second->m_info.m_xPos, b.second->m_info.m_zPos, b.second->m_info.m_angle, 0);
				}
		g_buildings_lock.unlock();
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

	double tick = (GetTickCount64() - g_clients[user_id]->m_last_move_time) * 0.001f;
	g_clients[user_id]->m_xPos += g_clients[user_id]->m_xVel * tick;
	g_clients[user_id]->m_zPos += g_clients[user_id]->m_zVel * tick;
	g_clients[user_id]->m_last_move_time = GetTickCount64();

	g_clients[user_id]->is_collide(prev_x, prev_z);

	if (g_clients[user_id]->m_xPos < 0.0f) g_clients[user_id]->m_xPos = 0.0f;
	if (g_clients[user_id]->m_zPos < 0.0f) g_clients[user_id]->m_zPos = 0.0f;
	if (g_clients[user_id]->m_xPos > 999.f) g_clients[user_id]->m_xPos = 999.f;
	if (g_clients[user_id]->m_zPos > 999.f) g_clients[user_id]->m_zPos = 999.f;

	if (true == g_clients[user_id]->is_sector_change(prev_x, prev_z)) {
		g_clients[user_id]->erase_client_in_sector(prev_x, prev_z);
		g_clients[user_id]->insert_client_in_sector();
	}

	g_clients[user_id]->m_cl.lock();
	unordered_set<int> old_vl = g_clients[user_id]->view_list;
	unordered_set<int> old_sl = g_clients[user_id]->sim_list;
	unordered_set<int> new_vl;
	unordered_set<int> new_sl;
	g_clients[user_id]->m_cl.unlock();

	vector<int> near_clients = g_clients[user_id]->get_near_clients();
	for (auto cl : near_clients) new_vl.insert(cl);

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
			g_clients[new_player]->m_cl.lock();
			if (g_clients[new_player]->view_list.count(user_id) == 0) {
				g_clients[new_player]->m_cl.unlock();
				iocp.send_enter_packet(new_player, user_id);
			}
			else {
				g_clients[new_player]->m_cl.unlock();
				iocp.send_move_packet(new_player, user_id, rotAngle);
			}
		}
		else {
			g_clients[new_player]->m_cl.lock();
			if (0 != g_clients[new_player]->view_list.count(user_id)) {
				g_clients[new_player]->m_cl.unlock();
				iocp.send_move_packet(new_player, user_id, rotAngle);
			}
			else {
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
			g_clients[old_player]->m_cl.lock();
			if (0 != g_clients[old_player]->view_list.count(user_id)) {
				g_clients[old_player]->m_cl.unlock();
				iocp.send_leave_packet(old_player, user_id);
			}
			else {
				g_clients[old_player]->m_cl.unlock();
			}
		}
	}

	vector<int> near_sims = g_clients[user_id]->get_near_sims();
	for (auto sl : near_sims) new_sl.insert(sl);

	for (auto new_sim : new_sl) {
		g_sims_lock.lock();
		if (g_sims.count(new_sim) == 0) {
			g_sims_lock.unlock();
			continue;
		}
		g_sims_lock.unlock();
		if (old_sl.count(new_sim) == 0) 
			iocp.send_enter_sim_packet(user_id, new_sim);
	}

	for (auto old_sim : old_sl) {
		g_sims_lock.lock();
		if (g_sims.count(old_sim) == 0) {
			g_sims_lock.unlock();
			continue;
		}
		g_sims_lock.unlock();
		if (new_sl.count(old_sim) == 0) 
			iocp.send_leave_sim_packet(user_id, old_sim);
	}
}

void Contents::teleport(int user_id, float xPos, float zPos)
{
	float prev_x = g_clients[user_id]->m_xPos;
	float prev_z = g_clients[user_id]->m_zPos;

	if (xPos >= WORLD_WIDTH || xPos < 0 || zPos >= WORLD_HEIGHT || zPos < 0)
		return;
	g_clients[user_id]->m_xPos = xPos;
	g_clients[user_id]->m_zPos = zPos;

	if (true == g_clients[user_id]->is_sector_change(prev_x, prev_z)) {
		g_clients[user_id]->erase_client_in_sector(prev_x, prev_z);
		g_clients[user_id]->insert_client_in_sector();
	}

	vector<pair<BuildingInfo, pair<int, int>>> near_buildings = g_clients[user_id]->get_near_buildings();
	g_buildings_lock.lock();
	for (auto cl : near_buildings)
		if (g_buildings[cl.second.first][cl.second.second][cl.first]->is_collide(g_clients[user_id]->m_xPos, g_clients[user_id]->m_zPos, g_clients[user_id]->m_rotAngle)) {
			g_clients[user_id]->m_collide_invincible = true;
			break;
		}
	g_buildings_lock.unlock();

	g_clients[user_id]->m_cl.lock();
	unordered_set<int> old_vl = g_clients[user_id]->view_list;
	unordered_set<int> old_sl = g_clients[user_id]->sim_list;
	unordered_set<int> new_vl;
	unordered_set<int> new_sl;
	g_clients[user_id]->m_cl.unlock();

	vector<int> near_clients = g_clients[user_id]->get_near_clients();
	for (auto cl : near_clients) new_vl.insert(cl);

	iocp.send_teleport_packet(user_id, user_id);
	iocp.send_teleport_packet(contents.host_id, user_id);

	for (auto new_player : new_vl) {
		g_clients_lock.lock();
		if (g_clients.count(new_player) == 0) {
			g_clients_lock.unlock();
			continue;
		}
		g_clients_lock.unlock();
		if (old_vl.count(new_player) == 0) {
			iocp.send_enter_packet(user_id, new_player);
			g_clients[new_player]->m_cl.lock();
			if (g_clients[new_player]->view_list.count(user_id) == 0) {
				g_clients[new_player]->m_cl.unlock();
				iocp.send_enter_packet(new_player, user_id);
			}
			else {
				g_clients[new_player]->m_cl.unlock();
				iocp.send_teleport_packet(new_player, user_id);
			}
		}
		else {
			g_clients[new_player]->m_cl.lock();
			if (0 != g_clients[new_player]->view_list.count(user_id)) {
				g_clients[new_player]->m_cl.unlock();
				iocp.send_teleport_packet(new_player, user_id);
			}
			else {
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
			g_clients[old_player]->m_cl.lock();
			if (0 != g_clients[old_player]->view_list.count(user_id)) {
				g_clients[old_player]->m_cl.unlock();
				iocp.send_leave_packet(old_player, user_id);
			}
			else {
				g_clients[old_player]->m_cl.unlock();
			}
		}
	}

	vector<int> near_sims = g_clients[user_id]->get_near_sims();
	for (auto sl : near_sims) new_sl.insert(sl);

	for (auto new_sim : new_sl) {
		g_sims_lock.lock();
		if (g_sims.count(new_sim) == 0) {
			g_sims_lock.unlock();
			continue;
		}
		g_sims_lock.unlock();
		if (old_sl.count(new_sim) == 0)
			iocp.send_enter_sim_packet(user_id, new_sim);
	}

	for (auto old_sim : old_sl) {
		g_sims_lock.lock();
		if (g_sims.count(old_sim) == 0) {
			g_sims_lock.unlock();
			continue;
		}
		g_sims_lock.unlock();
		if (new_sl.count(old_sim) == 0)
			iocp.send_leave_sim_packet(user_id, old_sim);
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
		for (auto cl = g_clients[user_id]->view_list.begin(); cl != g_clients[user_id]->view_list.end(); ++cl) {
			if (user_id == *cl) continue;
			if (ST_ACTIVE == g_clients[*cl]->m_status)
				iocp.send_leave_packet(*cl, user_id);
		}
		if (host_id != -1)
			iocp.send_leave_packet(host_id, user_id);
	}
	else {
		for (auto cl = g_clients.begin(); cl != g_clients.end();) {
			if (cl->first == user_id) { ++cl; continue; }
			if (ST_ACTIVE == cl->second->m_status) {
				cout << "Disconnect " << cl->second->m_id << endl;
				iocp.send_leave_packet(cl->first, cl->first);

				cl->second->m_cl.lock();
				cl->second->m_status = ST_ALLOC;

				linger sopt_linger = { 0, 0 };
				shutdown(g_clients[user_id]->m_s, SD_BOTH);
				setsockopt(g_clients[user_id]->m_s, SOL_SOCKET, SO_LINGER, (char*)&sopt_linger, sizeof(sopt_linger));
				closesocket(g_clients[user_id]->m_s);
				g_clients[user_id]->m_s = INVALID_SOCKET;

				cl->second->m_cl.unlock();
				delete cl->second;
				cl = g_clients.erase(cl);
			}
			else
				++cl;
		}

		lobby_server.send_host_logout_packet();
		init_contents();

		cout << "Disconnect the host" << endl;
	}
	g_clients[user_id]->m_cl.unlock();

	delete g_clients[user_id];
	g_clients.erase(user_id);
}

void Contents::chatting(int user_id, wchar_t mess[])
{
	g_clients_lock.lock();
	for (auto& cl : g_clients) {
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_chat_packet(cl.second->m_id, user_id, mess);
	}
	g_clients_lock.unlock();
}

void Contents::login_fail(int user_id)
{
	cout << "Login fail!! user_id : " << user_id << endl;

	iocp.send_login_fail_packet(user_id);
}

class Village* Contents::get_my_landmark(class Building* b)
{
	for (auto village : g_villages) {
		float dist = sqrt(pow(b->m_info.m_xPos - village->m_info.m_xPos, 2) + pow(b->m_info.m_zPos - village->m_info.m_zPos, 2));
		if (village->m_land_range >= dist)
			return village;
	}
	return nullptr;
}

void Contents::do_construct(int user_id, int b_type, int b_name, float xpos, float zpos, float angle, int land_range, bool develop)
{
	BuildingInfo b{ b_type, b_name, xpos, zpos, angle };
	pair<int, int> b_sectnum = calculate_sector_num(xpos, zpos);
	g_buildings_lock.lock();
	if (b_type == Landmark) {
		Village* landmark = new Village(b_type, b_name, xpos, zpos, angle, land_range);
		if(develop)
			landmark->OnAutoDevelopment();
		else
			landmark->OffAutoDevelopment();
		g_buildings[b_sectnum.second][b_sectnum.first][b] = landmark;
		g_villages.insert(landmark);
	}
	else {
		g_buildings[b_sectnum.second][b_sectnum.first][b] = new Building(b_type, b_name, xpos, zpos, angle);
		if (b_type != Nature) {
			Village* v = get_my_landmark(g_buildings[b_sectnum.second][b_sectnum.first][b]);
			v->buildingList.emplace_back(g_buildings[b_sectnum.second][b_sectnum.first][b]);
		}
	}
	g_buildings[b_sectnum.second][b_sectnum.first][b]->m_collider = collider_info[b_type][b_name];
	g_buildings[b_sectnum.second][b_sectnum.first][b]->update_terrain_node(true);

	g_buildings_lock.unlock();

	if (b_type == House) {

		g_sims_lock.lock();
		g_sims[sim_index] = new Sim(sim_index, xpos, zpos);
		g_sims[sim_index]->home = g_buildings[b_sectnum.second][b_sectnum.first][b];
		if(ingame_time > dawn_start_time)
			g_sims[sim_index]->stateMachine.PushState(IdleState::Instance());
		else
			g_sims[sim_index]->stateMachine.PushState(SleepState::Instance());
		g_sims[sim_index]->stateMachine.GetCurrentState()->Enter(g_sims[sim_index]);
		g_sims[sim_index]->insert_client_in_sector();

		g_buildings[b_sectnum.second][b_sectnum.first][b]->m_sim = g_sims[sim_index];
		Village* v = get_my_landmark(g_buildings[b_sectnum.second][b_sectnum.first][b]);
		v->simList.emplace_back(g_sims[sim_index]);

		cout << "Sim " << sim_index << " is created" << endl;
		iocp.send_enter_sim_packet(contents.host_id, sim_index);

		vector<int> near_clients = g_sims[sim_index]->get_near_clients();

		for (auto cl : near_clients) {
			if (ST_ACTIVE == g_clients[cl]->m_status) 
				iocp.send_enter_sim_packet(cl, sim_index);
		}

		++sim_index;
		g_sims_lock.unlock();
	}	

	vector<int> b_near_clients = g_buildings[b_sectnum.second][b_sectnum.first][b]->get_near_clients();
	for (auto cl : b_near_clients) {
		lock_guard<mutex>lock_guard(g_clients[cl]->m_cl);
		if (g_buildings[b_sectnum.second][b_sectnum.first][b]->is_collide(g_clients[cl]->m_xPos, g_clients[cl]->m_zPos, g_clients[cl]->m_rotAngle))
			g_clients[cl]->m_collide_invincible = true;
	}

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status) {
			iocp.send_construct_packet(cl.second->m_id, b_type, b_name, xpos, zpos, angle, land_range);
		}
	}

	// 건물에 sim_index를 부여하여 어떤 심이 사는 집인지 알 수 있게 해야 함
}

void Contents::do_destruct(int user_id, int b_type, int b_name, float xpos, float zpos, float angle)
{
	pair<int, int> b_sectnum = calculate_sector_num(xpos, zpos);
	BuildingInfo b{ b_type, b_name, xpos, zpos, angle };

	g_buildings_lock.lock();
	if (g_buildings[b_sectnum.second][b_sectnum.first].count(b) != 0) {
		Village* v = get_my_landmark(g_buildings[b_sectnum.second][b_sectnum.first][b]);
		if (b_type == Landmark) {
			// 랜드마크인지?
			// 랜드마크이면 포함되는 모든 건물과 모든 심을 제거해줘야 함
			// g_village에서도 삭제해야 함
			for (int i = 0; i < v->buildingList.size();++i) {
				if (v->buildingList[i]->m_sim != nullptr) {
					g_sims_lock.lock();
					v->buildingList[i]->m_sim->erase_sim_in_sector();
					int simID = v->eraseSim(v->buildingList[i]->m_sim);
					for (auto& cl : g_clients) {
						if (ST_ACTIVE == cl.second->m_status)
							iocp.send_leave_sim_packet(cl.second->m_id, simID);
					}
					delete g_sims[simID];
					g_sims.erase(simID);
					cout << "Sim " << simID << "is deleted" << endl;
					g_sims_lock.unlock();
				}
				BuildingInfo vb = v->buildingList[i]->m_info;
				pair<int, int> sect_num = calculate_sector_num(vb.m_xPos, vb.m_zPos);
				delete g_buildings[sect_num.second][sect_num.first][vb];
				g_buildings[sect_num.second][sect_num.first].erase(vb);
				cout << "Building " << vb.building_type << ", " << vb.building_name << "is deleted" << endl;
			}
			for (int i = 0; i < v->simList.size(); ++i) {
				if (v->simList[i] != nullptr) {
					g_sims_lock.lock();
					v->buildingList[i]->m_sim->erase_sim_in_sector();
					int simID = v->simList[i]->id;
					for (auto& cl : g_clients) {
						if (ST_ACTIVE == cl.second->m_status)
							iocp.send_leave_sim_packet(cl.second->m_id, simID);
					}
					delete g_sims[simID];
					g_sims.erase(simID);
					cout << "Sim " << simID << "is deleted" << endl;
					g_sims_lock.unlock();
				}
			}
			v->simList.clear();
			v->buildingList.clear();

			g_villages.erase(v);
			delete g_buildings[b_sectnum.second][b_sectnum.first][b];
			g_buildings[b_sectnum.second][b_sectnum.first].erase(b);
		}

		else if (b_type == House) {
			v->eraseBuilding(g_buildings[b_sectnum.second][b_sectnum.first][b]);
			Sim* sim = g_buildings[b_sectnum.second][b_sectnum.first][b]->m_sim;
			g_sims_lock.lock();
			sim->erase_sim_in_sector();
			int simID = v->eraseSim(sim);
			for (auto& cl : g_clients) {
				if (ST_ACTIVE == cl.second->m_status)
					iocp.send_leave_sim_packet(cl.second->m_id, simID);
			}
			delete g_sims[simID];
			g_sims.erase(simID);
			cout << "Sim " << simID << "is deleted" << endl;
			g_sims_lock.unlock();
			delete g_buildings[b_sectnum.second][b_sectnum.first][b];
			g_buildings[b_sectnum.second][b_sectnum.first].erase(b);
			cout << "Building " << b_type << ", " << b_name << "is deleted" << endl;
		}
		else if (b_type == Nature) {
			delete g_buildings[b_sectnum.second][b_sectnum.first][b];
			g_buildings[b_sectnum.second][b_sectnum.first].erase(b);
			cout << "Natrue " << b_type << ", " << b_name << "is deleted" << endl;
		}
		else {
			// 그냥 건물이면?
			// 얘만 삭제하고, 얘 포함한 랜드마크에서 얘 삭제하면 됨
			v->eraseBuilding(g_buildings[b_sectnum.second][b_sectnum.first][b]);
			delete g_buildings[b_sectnum.second][b_sectnum.first][b];
			g_buildings[b_sectnum.second][b_sectnum.first].erase(b);
			cout << "Building " << b_type << ", " << b_name << "is deleted" << endl;
		}
	}
	g_buildings_lock.unlock();

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_destruct_packet(cl.second->m_id, b_type, b_name, xpos, zpos);
	}
}

void Contents::destruct_all(int user_id)
{
	init_sector();
	init_buildings();
	init_sims();

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_destruct_all_packet(cl.second->m_id);
	}
}

void Contents::update()
{
	tick_count = (GetTickCount64() - server_time) / second;
	server_time = GetTickCount64();
	ingame_time += tick_count;

	if (ingame_time >= max_oneday){
		ingame_time -= max_oneday;
		wakeup_flag = false;
		sleep_flag = false;
		for (int i = 0; i < 4; ++i)
			send_packet_flag[i] = false;

		send_packet_flag[0] = true;
		timer_event ev = { 0, GAME_Time, high_resolution_clock::now(), 0, NULL };
		timer.add_event(ev);
	}
	else if (ingame_time > night_start_time) {
		if (!send_packet_flag[3]) {
			send_packet_flag[3] = true;
			timer_event ev = { 0, GAME_Time, high_resolution_clock::now(), 0, NULL };
			timer.add_event(ev);
		}
	}
	else if (ingame_time > day_start_time) {
		if (!send_packet_flag[2]) {
			send_packet_flag[2] = true;
			timer_event ev = { 0, GAME_Time, high_resolution_clock::now(), 0, NULL };
			timer.add_event(ev);
		}
	}
	else if (ingame_time > dawn_start_time) {
		if (!send_packet_flag[1]) {
			send_packet_flag[1] = true;
			timer_event ev = { 0, GAME_Time, high_resolution_clock::now(), 0, NULL };
			timer.add_event(ev);
		}
	}


	update_sim();

	if (host_id != -1) {
		timer_event ev = { 0, GAME_Update, high_resolution_clock::now() + milliseconds(333), 0, NULL };
		timer.add_event(ev);
	}
}

void Contents::update_sim()
{
	lock_guard<mutex> lock_guard(g_sims_lock);
	if (ingame_time > 0.f && !sleep_flag) {
		for (auto& sims : g_sims) {
			timer_event ev = { sims.first, SIM_Sleep, high_resolution_clock::now(), sims.first, NULL };
			timer.add_event(ev);
		}
		sleep_flag = true;
	}

	if (ingame_time > dawn_start_time && !wakeup_flag) {
		for (auto& sims : g_sims) {
			timer_event ev = { sims.first, SIM_WakeUp, high_resolution_clock::now(), sims.first, NULL };
			timer.add_event(ev);
		}
		wakeup_flag = true;
	}

	for (auto& landmark : g_villages){
		if (landmark->autoDevelopment && !landmark->simList.empty()) {
			if (landmark->delayTime <= 0.f) {
				BuildMessageInfo* info = new BuildMessageInfo;
				info->pos = Vector2D(landmark->m_info.m_xPos + (rand() % landmark->m_land_range) - (landmark->m_land_range / 2), landmark->m_info.m_zPos + (rand() % landmark->m_land_range) - (landmark->m_land_range / 2));
				info->buildingType = rand() % 2 + 3;
				info->buildingIndex = rand() % collider_info[info->buildingType].size();
				landmark->delayTime = rand() % 120 + 60;

				timer_event ev = { -1, SIM_Build,  high_resolution_clock::now(), rand() % landmark->simList.size(), info };
				timer.add_event(ev);
			}
			landmark->delayTime -= tick_count;
		}
	}

	for (auto& sims : g_sims) {
		sims.second->Update();
	}
}

pair<int, int> Contents::calculate_sector_num(float xPos, float zPos)
{
	int x_sectnum = floor(xPos / SECTOR_WIDTH);
	int z_sectnum = floor(zPos / SECTOR_WIDTH);
	
	return make_pair(x_sectnum, z_sectnum);
}