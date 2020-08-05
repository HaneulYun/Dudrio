#pragma once
#include "Sim.h"

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
	unordered_set<int> sim_list;

	float	m_xPos, m_yPos, m_zPos;
	float	m_rotAngle;
	float	m_xVel, m_zVel;
	wchar_t	m_name[MAX_ID_LEN + 1];

	bool	m_collide_invincible;

public:
	Client(SOCKET& sock, int id);
	Client(int id);
	~Client();

	bool	is_sector_change(float prevX, float prevZ);
	void	erase_client_in_sector(float x, float z);
	void	erase_client_in_sector();
	void	insert_client_in_sector();

	vector<int>	get_near_clients();
	vector<int> get_near_sims();
	vector<pair<BuildingInfo, pair<int, int>>> get_near_buildings();
	void	is_collide(float prevX, float prevZ);

	bool is_near(const Client& other)
	{
		if (fabs(m_xPos - other.m_xPos) > VIEW_RADIUS)	return false;
		if (fabs(m_zPos - other.m_zPos) > VIEW_RADIUS)	return false;
		return true;
	}

	bool is_near(const Sim& other)
	{
		if (fabs(m_xPos - other.pos.x) > VIEW_RADIUS)	return false;
		if (fabs(m_zPos - other.pos.z) > VIEW_RADIUS)	return false;
		return true;
	}
};