#pragma once
#include "main.h"
#define DEGTORAD(x) x*(3.141592654f / 180.0f)
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

	bool is_collide(float x, float z, float prevX, float prevZ)
	{
		// leftup 1, leftdown 2, rightdown 3, rightup 4
		pair<float, float> point[4];

		point[0] = make_pair(m_info.m_xPos + m_collider.m_x1, m_info.m_zPos + m_collider.m_z2);
		point[1] = make_pair(m_info.m_xPos + m_collider.m_x1, m_info.m_zPos + m_collider.m_z1);
		point[2] = make_pair(m_info.m_xPos + m_collider.m_x2, m_info.m_zPos + m_collider.m_z1);
		point[3] = make_pair(m_info.m_xPos + m_collider.m_x2, m_info.m_zPos + m_collider.m_z2);
	
		// 좌표 회전 코드 삽입하기

		double increase[5], constant[5], samevalue[5];
		pair<double, double> intersect_coord[4];
		double dist[4]; double min_dist = sqrt(pow(x - prevX, 2) + pow(z - prevZ, 2)); int min_index = 10;

		// 직선 방정식 -----------------------------------------
		if (x == prevX)
			samevalue[0] = prevX;
		else {
			increase[0] = (float)(z - prevZ) / (x - prevX);
			constant[0] = prevZ - increase[0] * prevX;
		}

		for (int i = 0; i < 4; ++i) {
			int j = i + 1;
			if (j > 3) j = 0;
			if (point[i].first == point[j].first)
				samevalue[i + 1] = point[i].first;
			else {
				increase[i + 1] = (float)(point[j].second - point[i].second) / (point[j].first - point[i].first);
				constant[i + 1] = point[i].second - increase[i + 1] * point[i].first;
			}
		}
		// -----------------------------------------------------
		for (int i = 0; i < 4; ++i) {
			int j = i + 1;
			if (j > 3)j = 0;
			if (x == prevX && point[i].first == point[j].first) { intersect_coord[i] = make_pair(-1, -1); continue; }
			if (x == prevX)
				intersect_coord[i] = make_pair(samevalue[0], increase[i + 1] * samevalue[0] + constant[i + 1]);
			else if (point[i].first == point[j].first)
				intersect_coord[i] = make_pair(samevalue[i + 1], increase[0] * samevalue[i + 1] + constant[0]);
			else {
				intersect_coord[i].first = -1 * (constant[0] - constant[i + 1]) / (increase[0] - increase[i + 1]);
				intersect_coord[i].second = increase[0] * intersect_coord[i].first + constant[0];
			}
		}

		for (int i = 0; i < 4; ++i) {
			dist[i] = sqrt(pow(intersect_coord[i].first - prevX, 2) + pow(intersect_coord[i].second - prevZ, 2));
			if (dist[i] <= min_dist) { return true; }
		}
		return false;
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

	bool	is_sector_change(float prevX, float prevZ);
	void	erase_client_in_sector(float x, float z);
	void	erase_client_in_sector();
	void	insert_client_in_sector();

	vector<int>	get_near_clients();
	vector<pair<BuildingInfo, pair<int, int>>> get_near_buildings(float x, float z);
	void	is_collide(float prevX, float prevZ);

	bool is_near(const Client& other)
	{
		if (fabs(m_xPos - other.m_xPos) > VIEW_RADIUS)	return false;
		if (fabs(m_zPos - other.m_zPos) > VIEW_RADIUS)	return false;
		return true;
	}
};