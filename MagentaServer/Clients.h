#pragma once
#include "main.h"

class Clients
{
public:
	RWLock	m_cl;
	SOCKET	m_s;
	int		m_id;
	EXOVER	m_recv_over;
	int		m_prev_size;
	char	m_packet_buf[MAX_PACKET_SIZE];
	atomic	<C_STATUS> m_status;
	unsigned m_move_time;
	high_resolution_clock::time_point m_last_move_time;

	unordered_set<int> view_list;
	
	bool	is_host;
	float	xPos, zPos;
	float	xVel, zVel;
	float	rotAngle;
	bool	moving;
	DWORD	startMoveTime;
	char	m_name[MAX_ID_LEN + 1];

public:
	Clients(SOCKET& sock, int id);
	Clients(int id);
	~Clients();

	void enter_game(char name[]);
	void do_move_start(float xPos, float zPos, float xMove, float zMove);
	void do_move_end();
	void do_rotate(float xPos, float zPos, float xMove, float zMove, float rAngle);
};