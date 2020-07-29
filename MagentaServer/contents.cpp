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
	timer.clear_timer();

	host_id = -1;
	tick_count = 0.f;
	ingame_time = 0.f;
	tmp_sleep_time = 0.f;

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
}

void Contents::process_packet(int user_id, char* buf)
{
	switch (buf[1]) {
	case C2S_LOGIN_GUEST: {
		if (host_id != -1) {
			cout << "The Guest " << user_id << " is connected" << endl;
			cs_packet_login_guest* packet = reinterpret_cast<cs_packet_login_guest*>(buf);
			enter_game(user_id, packet->name);
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

			update();
			enter_game(user_id, packet->name);
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
			do_construct(user_id, packet->building_type, packet->building_name, packet->xpos, packet->zpos, packet->angle);
	}
	break;
	case C2S_DESTRUCT:
	{
		cs_packet_destruct* packet = reinterpret_cast<cs_packet_destruct*>(buf);
		if (user_id == host_id)
			do_destruct(user_id);
	}
	break;
	case C2S_DESTRUCT_ALL:
	{
		cs_packet_destruct_all* packet = reinterpret_cast<cs_packet_destruct_all*>(buf);
		if (user_id == host_id)
			destruct_all(user_id);
	}
	break;
	case C2S_CHAT:
	{

	}
	break;
	default:
		cout << "Unknown Packet Type Error!\n" << user_id << ", " << buf << endl;
		//DebugBreak();
		//exit(-1);
		break;
	}
}


void Contents::enter_game(int user_id, char name[])
{
	g_clients[user_id]->m_cl.lock();
	strcpy_s(g_clients[user_id]->m_name, name);
	g_clients[user_id]->m_name[MAX_ID_LEN] = NULL;
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
			if (g_buildings[cl.second.first][cl.second.second][cl.first]->is_collide(g_clients[user_id]->m_xPos, g_clients[user_id]->m_zPos, g_clients[user_id]->m_rotAngle))
				g_clients[user_id]->m_collide_invincible = true;
		g_buildings_lock.unlock();
		g_clients[user_id]->m_cl.unlock();

		for (auto cl : near_clients) {
			if (ST_ACTIVE == g_clients[cl]->m_status) {
				iocp.send_enter_packet(user_id, cl);
				iocp.send_enter_packet(cl, user_id);
			}
		}
		
		g_buildings_lock.lock();
		for(int i=0;i< WORLD_HEIGHT / SECTOR_WIDTH;++i)
			for(int j=0;j< WORLD_WIDTH / SECTOR_WIDTH;++j)
				for (auto b : g_buildings[i][j])
					iocp.send_construct_packet(user_id, b.second->m_info.building_type, b.second->m_info.building_name, b.second->m_info.m_xPos, b.second->m_info.m_zPos, b.second->m_info.m_angle);
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

	g_clients[user_id]->m_xPos += g_clients[user_id]->m_xVel * (GetTickCount64() - g_clients[user_id]->m_last_move_time) / 1000.f;
	g_clients[user_id]->m_zPos += g_clients[user_id]->m_zVel * (GetTickCount64() - g_clients[user_id]->m_last_move_time) / 1000.f;
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

	iocp.send_login_fail_packet(user_id);
}

void Contents::do_construct(int user_id, int b_type, int b_name, float xpos, float zpos, float angle)
{
	BuildingInfo b{ b_type, b_name, xpos, zpos, angle };
	pair<int, int> b_sectnum = calculate_sector_num(xpos, zpos);
	g_buildings_lock.lock();
	if (b_type == Landmark) {
		Village* landmark = new Village(b_type, b_name, xpos, zpos, angle);
		g_buildings[b_sectnum.second][b_sectnum.first][b] = landmark;
		g_villages.insert(landmark);
	}
	else
		g_buildings[b_sectnum.second][b_sectnum.first][b] = new Building(b_type, b_name, xpos, zpos, angle);
	g_buildings[b_sectnum.second][b_sectnum.first][b]->m_collider = collider_info[b_type][b_name];
	g_buildings[b_sectnum.second][b_sectnum.first][b]->update_terrain_node(true);

	g_buildings_lock.unlock();

	if (b_type == House) {
		g_sims_lock.lock();
		g_sims[sim_index] = new Sim(sim_index, xpos, zpos);
		g_sims[sim_index]->home = g_buildings[b_sectnum.second][b_sectnum.first][b];
		g_sims[sim_index]->stateMachine.PushState(IdleState::Instance());
		g_sims[sim_index]->stateMachine.GetCurrentState()->Enter(g_sims[sim_index]);
		g_sims[sim_index]->insert_client_in_sector();
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

	g_sector_clients_lock[b_sectnum.second][b_sectnum.first].lock();
	for (auto cl : g_sector_clients[b_sectnum.second][b_sectnum.first]) {
		lock_guard<mutex>lock_guard(cl->m_cl);
		if (g_buildings[b_sectnum.second][b_sectnum.first][b]->is_collide(cl->m_xPos, cl->m_zPos, cl->m_rotAngle)) 
			cl->m_collide_invincible = true;
	}
	g_sector_clients_lock[b_sectnum.second][b_sectnum.first].unlock();

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_construct_packet(cl.second->m_id, b_type, b_name, xpos, zpos, angle);
	}
	// 건설 타입이 House일 경우 클라이언트에서 심이 enter된 것으로 인식하게 처리해야 함
	// 건물에 sim_index를 부여하여 어떤 심이 사는 집인지 알 수 있게 해야 함
}

void Contents::do_destruct(int user_id)
{
	//pair<int, int> b_sectnum = calculate_sector_num(x, z);
	//Building b{ type, name, x, z, angle };
	//if (g_buildings[b_sectnum.second][b_sectnum.first].count(b) != 0) {
	//	cout << "destruct" << endl;
	//	delete g_buildings[b_sectnum.second][b_sectnum.first][b];
	//	g_buildings[b_sectnum.second][b_sectnum.first].erase(b);
	//}

	// 건물이 house일 경우 심 삭제도 해야 함
	// 건물이 랜드마크일 경우 처리도 해주자 (g_village에서 삭제 및 랜드마크 안 모든 건물 및 심 삭제)
	//for (auto& cl : g_clients){
	//	if (user_id == cl.second->m_id)
	//		continue;
	//	if (ST_ACTIVE == cl.second->m_status)
	//		iocp.send_destruct_packet(cl.second->m_id, b_inform);
	//}
}

void Contents::destruct_all(int user_id)
{
	init_buildings();

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_destruct_all_packet(cl.second->m_id);
	}
}

void Contents::update()
{
	tick_count = GetTickCount64() - ingame_time;
	ingame_time = GetTickCount64();
	tmp_sleep_time += tick_count;

	update_sim();

	if (host_id != -1) {
		timer_event ev = { 0, GAME_Update, high_resolution_clock::now() + milliseconds(333), 0, NULL };
		timer.add_event(ev);
	}
}

void Contents::update_sim()
{
	lock_guard<mutex> lock_guard(g_sims_lock);
	//if (tmp_sleep_time > 30000.f) {
	//	for (auto& sims : g_sims) {
	//		timer_event ev = { sims.first, SIM_Sleep, high_resolution_clock::now(), sims.first, NULL };
	//		timer.add_event(ev);
	//	}
	//	tmp_sleep_time -= 30000.f;
	//}

	for (auto& landmark : g_villages){
		if (landmark->autoDevelopment && !landmark->simList.empty()) {
			if (landmark->delayTime <= 0.f) {
				BuildMessageInfo* info = new BuildMessageInfo;
				info->pos = Vector2D(landmark->m_info.m_xPos + (rand() % 30) - 15, landmark->m_info.m_zPos + (rand() % 30) - 15);
				info->buildingType = rand() % 2 + 3;
				info->buildingIndex = rand() % collider_info[info->buildingType].size();
				landmark->delayTime = rand() % 10 + 30;

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