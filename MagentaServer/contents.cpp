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

void Contents::start_contents()
{
	logic_run = true;
	logic_thread = thread([this]() { logic_thread_loop(); });
}

void Contents::stop_contents()
{
	logic_run = false;
	if (logic_thread.joinable())
		logic_thread.join();
}

void Contents::logic_thread_loop()
{
	while (logic_run){
		if (!recvQueue.empty()){
			logic_lock.EnterWriteLock();
			auto buf = recvQueue.front();
			recvQueue.pop();
			logic_lock.LeaveWriteLock();

			switch (buf.second[1]){
			case C2S_LOGIN:{
				if (host_id != -1){
					cout << "The Guest " <<  buf.first << "is connected" << endl;
					cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf.second);
					g_clients[buf.first]->is_host = false;
					g_clients[buf.first]->enter_game(packet->name);
				}
				else{
					cout << "Host does not exist" << endl;
					disconnect(buf.first);
				}
			}
			break;
			case C2S_LOGIN_HOST:
			{
				if (host_id == -1){
					cout << "The host is connected" << endl;
					cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf.second);
					host_id = buf.first;
					g_clients[buf.first]->is_host = true;
					g_clients[buf.first]->enter_game(packet->name);
				}
				else{
					cout << "Host is already exist" << endl;
					disconnect(buf.first);
				}
			}
			break;
			case C2S_MOVE_START:
			{
				cs_packet_move_start* packet = reinterpret_cast<cs_packet_move_start*>(buf.second);
				g_clients[buf.first]->m_last_move_time = GetTickCount64();
				g_clients[buf.first]->do_move(packet->xVel, packet->zVel, packet->rotAngle, packet->run_level);
			}
			break;
			case C2S_MOVE:
			{
				cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf.second);
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
			default:
				cout << "Unknown Packet Type Error!\n";
				DebugBreak();
				exit(-1);
				break;
			}
		}
		else
			this_thread::sleep_for(chrono::milliseconds(2));
	}
}

void Contents::disconnect(int user_id)
{
	g_clients[user_id]->m_status = ST_ALLOC;
	cout << "Disconnect " << user_id << endl;
	iocp.send_leave_packet(user_id, user_id);
	closesocket(g_clients[user_id]->m_s);

	delete g_clients[user_id];
	g_clients.erase(user_id);

	if (host_id != user_id){
		for (auto& cl : g_clients){
			if (user_id == cl.second->m_id)
				continue;
			if (ST_ACTIVE == cl.second->m_status)
				iocp.send_leave_packet(cl.second->m_id, user_id);
		}
	}
	else{
		cout << "Disconnect the host" << endl;
		for (auto& cl : g_clients){
			if (user_id == cl.second->m_id)
				continue;
			iocp.send_leave_packet(cl.second->m_id, cl.second->m_id);
			delete cl.second;
		}
		host_id = -1;
		buildings.clear();
		g_clients.clear();
	}
}

void Contents::do_construct(int user_id, BuildingInform b_inform)
{
	buildings.emplace_back(b_inform);
	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_construct_packet(cl.second->m_id, b_inform);
	}
}

void Contents::do_destruct(int user_id, BuildingInform b_inform)
{
	for (auto& b : buildings){
		if (b == b_inform){
			b = buildings.back();
			buildings.pop_back();
			break;
		}
	}

	for (auto& cl : g_clients){
		if (user_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_destruct_packet(cl.second->m_id, b_inform);
	}
}

void Contents::destruct_all(int user_id)
{
	buildings.clear();
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