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
	DWORD	m_last_move_time;
	unordered_set<int> view_list;
	
	bool	is_host;
	float	m_xPos, m_zPos;
	float	m_xVel, m_zVel;
	float	m_rotAngle;
	char	m_name[MAX_ID_LEN + 1];

public:
	Clients(SOCKET& sock, int id);
	Clients(int id);
	~Clients();

	void enter_game(char name[]);
	void do_move(float xVel, float zVel, float rotAngle, float run_level);
};