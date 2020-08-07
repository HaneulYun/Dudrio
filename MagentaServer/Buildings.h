#pragma once
#include "Terrain.h"

struct BuildingInfo {
	int		building_type;
	int		building_name;
	float	m_xPos, m_zPos;
	float	m_angle;

	BuildingInfo() {}
	BuildingInfo(int b_type, int b_name, float xpos, float zpos, float angle) 
	: building_type(b_type), building_name(b_name), m_xPos(xpos), m_zPos(zpos), m_angle(angle){}
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
	BuildingInfoHasher() {}
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

	Sim* m_sim{ nullptr };

public:
	Building() {}
	Building(int type, int name, float x, float z, float angle)
	{
		m_info.building_type = type;
		m_info.building_name = name;
		m_info.m_xPos = x;
		m_info.m_zPos = z;
		m_info.m_angle = angle;
	}

	virtual vector<int>	get_near_clients()
	{
		pair<int, int> sect_num = contents.calculate_sector_num(m_info.m_xPos, m_info.m_zPos);
		vector<int> near_clients;
		near_clients.clear();

		for (int i = sect_num.second - 1; i <= sect_num.second + 1; ++i) {
			if (i < 0 || i > WORLD_HEIGHT / SECTOR_WIDTH - 1) continue;
			for (int j = sect_num.first - 1; j <= sect_num.first + 1; ++j) {
				if (j < 0 || j > WORLD_WIDTH / SECTOR_WIDTH - 1) continue;
				lock_guard<mutex>lock_guard(g_sector_clients_lock[i][j]);
				for (auto nearObj : g_sector_clients[i][j]) {
					if (ST_ACTIVE != nearObj->m_status)	continue;
					if (true == m_info.is_near(nearObj->m_xPos, nearObj->m_zPos))
						near_clients.emplace_back(nearObj->m_id);
				}
			}
		}
		return near_clients;
	}

	virtual ~Building() { update_terrain_node(false); }

	virtual bool is_collide(float player_x, float player_z, float player_angle)
	{
		Vector2D dist_vector = getDistanceVector(m_info.m_xPos, m_info.m_zPos, player_x, player_z);
		Vector2D vectors[4];
		vectors[0] = getHeightVector(m_collider.m_z1, m_collider.m_z2, m_info.m_angle);
		vectors[1] = getHeightVector(player_z - 0.25, player_z + 0.25, player_angle);
		vectors[2] = getWidthVector(m_collider.m_x1, m_collider.m_x2, m_info.m_angle);
		vectors[3] = getWidthVector(player_x - 0.25, player_x + 0.25, player_angle);

		for (int i = 0; i < 4; ++i) {
			double sum = 0;
			Vector2D unit_vector = vectors[i].Normalize();
			for (int j = 0; j < 4; ++j)
				sum += dotproduct(vectors[j], unit_vector);
			if (dotproduct(dist_vector, unit_vector) > sum)
				return false;
		}

		return true;
	}

	virtual void update_terrain_node(bool create)
	{
		Vector2D b_right = (Vector2D(1, 0).Rotate(-m_info.m_angle)).Normalize();
		Vector2D b_forward = (Vector2D(0, 1).Rotate(-m_info.m_angle)).Normalize();

		for (int x = m_info.m_xPos + (b_right.x * m_collider.m_x1); x <= m_info.m_xPos + (b_right.x * m_collider.m_x2); ++x) {
			for (int z = m_info.m_zPos + (b_forward.z * m_collider.m_z1); z <= m_info.m_zPos + (b_forward.z * m_collider.m_z2); ++z)
			{
				if (is_collide(x, z, 0)) {
					terrain_data->extraData[x + (z * terrain_data->heightmapHeight)].collision = create;
					if (create)
						cout << "Create Node on x: " << x << ", z: " << z << endl;
					else
						cout << "Delete Node on x: " << x << ", z: " << z << endl;
				}
			}
		}
	}
};

class Village : public Building
{
public:
	//				Home		Sim
	//unordered_map<struct BuildingInfo, class Sim*, struct BuildingInfoHasher> simList;
	vector<class Sim*> simList;
	vector<class Building*> buildingList;

	float delayTime = 0.f;	// °Ç¼³ ÄðÅ¸ÀÓ
	bool autoDevelopment;
	int m_land_range;

public:
	Village(int type, int name, float x, float z, float angle, int land_range)
	{
		autoDevelopment = false;
		delayTime = 0.f;
		m_info.building_type = type;
		m_info.building_name = name;
		m_info.m_xPos = x;
		m_info.m_zPos = z;
		m_info.m_angle = angle;
		m_land_range = land_range;
	}

	virtual ~Village() {}

	void OnAutoDevelopment()
	{
		autoDevelopment = true;
		delayTime = rand() % 10 + 10;
	}

	void OffAutoDevelopment()
	{
		autoDevelopment = false;
	}

	void eraseBuilding(class Building* b)
	{
		auto iter = find(buildingList.begin(), buildingList.end(), b);
		if (iter != buildingList.end())
			buildingList.erase(iter);
	}

	int eraseSim(class Sim* s)
	{
		auto iter = find(simList.begin(), simList.end(), s);
		if (iter != simList.end())
			simList.erase(iter);
		return s->id;
	}
};
