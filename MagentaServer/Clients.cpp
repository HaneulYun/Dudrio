#include "Clients.h"

Clients::Clients(SOCKET& sock, int id)
{
	m_id = id;
	m_prev_size = 0;
	m_recv_over.op = OP_RECV;
	ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
	m_recv_over.wsabuf.buf = m_recv_over.io_buf;
	m_recv_over.wsabuf.len = MAX_BUF_SIZE;
	m_status = ST_FREE;
	m_s = sock;
	m_xPos = 540.0;	m_zPos = 540.0;
	m_xVel = 0.0;	m_zVel = 0.0;
	m_rotAngle = 0.0f;
}

Clients::Clients(int id)
{
	m_id = id;
	m_prev_size = 0;
	m_recv_over.op = OP_RECV;
	ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
	m_recv_over.wsabuf.buf = m_recv_over.io_buf;
	m_recv_over.wsabuf.len = MAX_BUF_SIZE;
	m_status = ST_FREE;
	m_xPos = 540.0;	m_zPos = 540.0;
	m_xVel = 0.0;	m_zVel = 0.0;
	m_rotAngle = 0.0f;
}

Clients::~Clients()
{

}

void Clients::enter_game(char name[])
{
	strcpy_s(m_name, name);
	m_name[MAX_ID_LEN] = NULL;
	iocp.send_login_ok_packet(m_id);

	for (auto& cl : g_clients){
		if (m_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status) {
			iocp.send_enter_packet(m_id, cl.second->m_id);
			iocp.send_enter_packet(cl.second->m_id, m_id);
		}
	}

	for (auto& b : buildings)
		iocp.send_construct_packet(m_id, b);

	m_status = ST_ACTIVE;
}


void Clients::do_move(float xVel, float zVel, float rotAngle, float run_level)
{
	// run_level == 2 : default
	m_rotAngle += rotAngle;

	float nomalize_vel = sqrt(pow(xVel, 2) + pow(zVel, 2));
	if (nomalize_vel != 0.0f) {
		m_xVel = xVel * run_level * 0.25 / nomalize_vel;
		m_zVel = zVel * run_level * 0.25 / nomalize_vel;
	}
	else {
		m_xVel = 0.0f;
		m_zVel = 0.0f;
	}
	m_xPos += m_xVel * (GetTickCount64() - m_last_move_time) / 1000.f;
	m_zPos += m_zVel * (GetTickCount64() - m_last_move_time) / 1000.f;
	m_last_move_time = GetTickCount64();

	for (auto& cl : g_clients)
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_move_packet(cl.second->m_id, m_id, rotAngle);
}