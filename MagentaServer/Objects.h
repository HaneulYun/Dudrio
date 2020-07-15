#pragma once
#include "main.h"

class Object
{
public:
	float	m_xPos, m_yPos, m_zPos;
	float	m_rotAngle;

public:
	Object() {}
	~Object() {}
};

class Building : public Object
{
public:
	BuildingType m_type;
	
	float m_halfwidth;
	float m_halfLength;

public:
	Building(BuildingInform b_inform)
	{
		m_type = b_inform.buildingType;
		m_xPos = b_inform.xPos;
		m_yPos = b_inform.yPos;
		m_zPos = b_inform.zPos;
	}

	~Building() {}

	bool operator== (const Building& b) const
	{
		return ((m_xPos == b.m_xPos) && (m_yPos == b.m_yPos)
			&& (m_zPos == b.m_zPos));
	}
};

class Client : public Object
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
	//float	m_xPos, m_zPos;
	float	m_xVel, m_zVel;
	//float	m_rotAngle;
	char	m_name[MAX_ID_LEN + 1];

public:
	Client(SOCKET& sock, int id);
	Client(int id);
	~Client();

	void enter_game(char name[]);
	void do_move(float xVel, float zVel, float rotAngle, float run_level);
};