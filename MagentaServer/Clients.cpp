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
	xPos = 540.0;	zPos = 540.0;
	xVel = 0.0;		zVel = 0.0;
	rotAngle = 0.0f;
	moving = false;
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
	xPos = 540.0;	zPos = 540.0;
	xVel = 0.0;		zVel = 0.0;
	rotAngle = 0.0f;
	moving = false;
}

Clients::~Clients()
{

}

void Clients::enter_game(char name[])
{
	strcpy_s(m_name, name);
	m_name[MAX_ID_LEN] = NULL;
	iocp.send_login_ok_packet(m_id);

	for (auto& cl : g_clients)
	{
		if (m_id == cl.second->m_id)
			continue;
		if (ST_ACTIVE == cl.second->m_status) {
			if (true == cl.second->moving) {
				cl.second->xPos += cl.second->xVel * (GetTickCount64() - cl.second->startMoveTime);
				cl.second->zPos += cl.second->zVel * (GetTickCount64() - cl.second->startMoveTime);
				cl.second->startMoveTime = GetTickCount64();
			}
			iocp.send_enter_packet(m_id, cl.second->m_id);
			iocp.send_enter_packet(cl.second->m_id, m_id);
			if (true == cl.second->moving) {
				iocp.send_move_start_packet(m_id, cl.second->m_id);
			}
		}
	}

	for (auto& b : buildings)
		iocp.send_construct_packet(m_id, b);

	m_status = ST_ACTIVE;
}

void Clients::do_move_start(float xPos, float zPos, float xMove, float zMove)
{
	float nmlzeSpeedNum = sqrt(pow(xMove, 2) + pow(zMove, 2));

	xVel = xMove / nmlzeSpeedNum;
	zVel = zMove / nmlzeSpeedNum;

	startMoveTime = GetTickCount64();
	moving = true;

	for (auto& cl : g_clients)
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_move_start_packet(cl.second->m_id, m_id);
}

void Clients::do_move_end()
{
	xPos += xVel * (GetTickCount64() - startMoveTime) / 1000.f;
	zPos += zVel * (GetTickCount64() - startMoveTime) / 1000.f;

	moving = false;

	for (auto& cl : g_clients)
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_move_end_packet(cl.second->m_id, m_id);
}

void Clients::do_rotate(float xPos, float zPos, float xMove, float zMove, float rAngle)
{
	rotAngle += rAngle;
	float nmlzeSpeedNum = sqrt(pow(xMove, 2) + pow(zMove, 2));
	if (nmlzeSpeedNum != 0.0f) {
		xVel = xMove / nmlzeSpeedNum;
		zVel = zMove / nmlzeSpeedNum;
	}
	else {
		xVel = 0.0f;
		zVel = 0.0f;
	}
	xPos += xVel * (GetTickCount64() - startMoveTime) / 1000.f;
	zPos += zVel * (GetTickCount64() - startMoveTime) / 1000.f;
	startMoveTime = GetTickCount64();

	for (auto& cl : g_clients)
		if (ST_ACTIVE == cl.second->m_status)
			iocp.send_rotate_packet(cl.second->m_id, m_id, rAngle);
}