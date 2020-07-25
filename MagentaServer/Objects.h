#pragma once
#include "main.h"

//struct BuildingInform
//{
//	BuildingType buildingType;
//	float xPos, yPos, zPos;
//	float rotAngle;
//
//	bool operator== (const BuildingInform& b) const
//	{
//		return ((xPos == b.xPos) && (yPos == b.yPos)
//			&& (zPos == b.zPos));
//	}
//
//	friend std::ostream& operator<<(std::ostream& os, const BuildingInform& b);
//	friend std::istream& operator>>(std::istream& in, BuildingInform& b);
//};
//
//struct BuildingInformHasher
//{
//	std::size_t operator()(const BuildingInform& b) const
//	{
//		using std::size_t;
//		using std::hash;
//
//		return ((hash<float>()(b.xPos)
//			^ (hash<float>()(b.yPos) << 1)) >> 1) ^ (hash<float>()(b.zPos) << 1);
//	}
//};
//
//static std::ostream& operator<<(std::ostream& os, const BuildingInform& b)
//{
//	os << static_cast<int>(b.buildingType) << " " << b.xPos << " " << b.yPos << " " << b.zPos << " " << b.rotAngle << std::endl;
//	return os;
//}
//
//static std::istream& operator>>(std::istream& in, BuildingInform& b)
//{
//	int input;
//	in >> input >> b.xPos >> b.yPos >> b.zPos >> b.rotAngle;
//	b.buildingType = static_cast<BuildingType>(input);
//	return in;
//}

struct Collider {
	float m_x1, m_z1;
	float m_x2, m_z2;

	Collider() {}
	Collider(float x1, float z1, float x2, float z2)
	{
		m_x1 = x1; m_z1 = z1; m_x2 = x2; m_z2 = z2;
	}
};

struct BuildingInfo {
	int		building_type;
	int		building_name;
	float	m_xPos, m_zPos;
	float	m_angle;

	bool operator== (const BuildingInfo& b) const
	{
		return ((building_type == b.building_type) && (building_name == b.building_name)
			&& (m_xPos == b.m_xPos) && (m_zPos == b.m_zPos)
			&& (m_angle == b.m_angle));
	}

	bool is_near(float x, float z) const
	{
		if (fabs(m_xPos - x) > 10)	return false;
		if (fabs(m_zPos - z) > 10)	return false;
		return true;
	}
};

struct BuildingInfoHasher
{
	std::size_t operator()(const BuildingInfo& b) const
	{
		using std::size_t;
		using std::hash;

		return ((hash<float>()(b.m_xPos)
			^ (hash<float>()(b.m_zPos) << 1)) >> 1) ^ (hash<float>()(b.m_angle) << 1);
	}
};

class Building 
{
public:
	BuildingInfo	m_info;
	Collider		m_collider;

public:
	Building(int type, int name, float x, float z, float angle)
	{
		m_info.building_type = type;
		m_info.building_name = name;
		m_info.m_xPos = x;
		m_info.m_zPos = z;
		m_info.m_angle = angle;
	}

	~Building() {}

	bool is_collide(float player_x, float player_z, float player_angle);
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
	vector<pair<BuildingInfo, pair<int, int>>> get_near_buildings(float x, float z);
	vector<pair<BuildingInfo, pair<int, int>>> get_near_buildings();
	void	is_collide(float prevX, float prevZ);

	bool is_near(const Client& other)
	{
		if (fabs(m_xPos - other.m_xPos) > VIEW_RADIUS)	return false;
		if (fabs(m_zPos - other.m_zPos) > VIEW_RADIUS)	return false;
		return true;
	}
};