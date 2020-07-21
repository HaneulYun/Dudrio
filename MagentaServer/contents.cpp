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
	logic_thread = thread([this]() { logic_thread_loop(); });

	init_contents();
}

void Contents::stop_contents()
{
	logic_run = false;
	if (logic_thread.joinable())
		logic_thread.join();
}

void Contents::logic_thread_loop()
{
	while (logic_run) {
		logic_lock.EnterReadLock();
		if (!recvQueue.empty()) {
			logic_lock.LeaveReadLock();
			logic_lock.EnterWriteLock();
			auto buf = recvQueue.front();
			recvQueue.pop();
			logic_lock.LeaveWriteLock();

			switch (buf.second[1]) {
			case C2S_LOGIN_GUEST: {
				if (host_id != -1) {
					cout << "The Guest " << buf.first << " is connected" << endl;
					cs_packet_login_guest* packet = reinterpret_cast<cs_packet_login_guest*>(buf.second);
					g_clients[buf.first]->is_host = false;
					g_clients[buf.first]->enter_game(packet->name);
				}
				else {
					cout << "Host does not exist" << endl;
					disconnect(buf.first);
				}
			}
			break;
			case C2S_LOGIN_HOST:
			{
				if (host_id == -1) {
					cout << "The host " << buf.first << " is connected" << endl;
					cs_packet_login_host* packet = reinterpret_cast<cs_packet_login_host*>(buf.second);
					host_id = buf.first;
					g_clients[buf.first]->is_host = true;

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

					g_clients[buf.first]->enter_game(packet->name);
				}
				else {
					cout << "Host is already exist" << endl;
					disconnect(buf.first);
				}
			}
			break;
			case C2S_MOVE_START:
			{
				cs_packet_move_start* packet = reinterpret_cast<cs_packet_move_start*>(buf.second);
				g_clients[buf.first]->m_last_move_time = GetTickCount64();
				g_clients[buf.first]->m_move_time = packet->move_time;
				g_clients[buf.first]->do_move(packet->xVel, packet->zVel, packet->rotAngle, packet->run_level);
			}
			break;
			case C2S_MOVE:
			{
				cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf.second);
				g_clients[buf.first]->m_move_time = packet->move_time;
				g_clients[buf.first]->do_move(packet->xVel, packet->zVel, packet->rotAngle, packet->run_level);
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
				cout << "Unknown Packet Type Error!\n";
				DebugBreak();
				exit(-1);
				break;
			}
		}
		else
			logic_lock.LeaveReadLock();
	}
}

void Contents::disconnect(int user_id)
{
	cout << "Disconnect " << user_id << endl;
	
	if (host_id != user_id)
		g_clients[user_id]->erase_client_in_sector();
	iocp.send_leave_packet(user_id, user_id);
	g_clients[user_id]->m_status = ST_ALLOC;
	closesocket(g_clients[user_id]->m_s);

	delete g_clients[user_id];
	g_clients.erase(user_id);

	if (host_id != user_id){
		for (auto cl : g_clients){
			if (user_id == cl.second->m_id)
				continue;
			if (ST_ACTIVE == cl.second->m_status)
				iocp.send_leave_packet(cl.second->m_id, user_id);
		}
	}
	else{
		cout << "Disconnect the host" << endl;
		for (auto cl = g_clients.begin(); cl != g_clients.end();) {
			if (ST_ACTIVE == cl->second->m_status) {
				cout << "Disconnect " << cl->second->m_id << endl;
				iocp.send_leave_packet(cl->second->m_id, cl->second->m_id);
				delete cl->second;
				cl = g_clients.erase(cl);
			}
			else
				++cl;
		}

		host_id = -1;

		init_contents();
	}
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
	logic_lock.EnterWriteLock();
	recvQueue.push(make_pair(user_id, buf));
	logic_lock.LeaveWriteLock();
}

pair<int, int> Contents::calculate_sector_num(float xPos, float zPos)
{
	int x_sectnum = floor(xPos / SECTOR_WIDTH);
	int z_sectnum = floor(zPos / SECTOR_WIDTH);
	
	return make_pair(x_sectnum, z_sectnum);
}