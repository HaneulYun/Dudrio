#pragma once
#include "main.h"

//class Object
//{
//public:
//	float	m_xPos, m_yPos, m_zPos;
//	float	m_rotAngle;
//
//public:
//	Object() {}
//	~Object() {}
//
//	bool is_near(const Object& other)
//	{
//		if (fabs(m_xPos - other.m_xPos) > VIEW_RADIUS)	return false;
//		if (fabs(m_zPos - other.m_zPos) > VIEW_RADIUS)	return false;
//		return true;
//	}
//};

class Building //: public Object
{
public:
	BuildingType m_type;
	
	float	m_xPos, m_yPos, m_zPos;
	float	m_rotAngle;
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

	bool is_near(const Building& other)
	{
		if (fabs(m_xPos - other.m_xPos) > VIEW_RADIUS)	return false;
		if (fabs(m_zPos - other.m_zPos) > VIEW_RADIUS)	return false;
		return true;
	}
};

class Client //: public Object
{
public:
	//RWLock	m_cl;
	mutex	m_cl;
	SOCKET	m_s;
	int		m_id;
	EXOVER	m_recv_over;
	int		m_prev_size;
	char	m_packet_buf[MAX_PACKET_SIZE];
	atomic	<C_STATUS> m_status;
	unsigned m_move_time;
	DWORD	m_last_move_time;
	unordered_set<int> view_list;

	float	m_xPos, m_yPos, m_zPos;
	float	m_rotAngle;
	float	m_xVel, m_zVel;
	char	m_name[MAX_ID_LEN + 1];

public:
	Client(SOCKET& sock, int id);
	Client(int id);
	~Client();

	bool is_sector_change(float prevX, float prevZ);
	void erase_client_in_sector(float x, float z);
	void erase_client_in_sector();
	void insert_client_in_sector();

	vector<int> get_near_clients();

	bool is_near(const Client& other)
	{
		if (fabs(m_xPos - other.m_xPos) > VIEW_RADIUS)	return false;
		if (fabs(m_zPos - other.m_zPos) > VIEW_RADIUS)	return false;
		return true;
	}
};