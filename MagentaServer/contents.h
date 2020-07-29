#pragma once

class Contents
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Count, None = -1 };
	vector<struct Collider> collider_info[BuildingType::Count];

	int		host_id;
	int		sim_index;
	float	ingame_time;
	float	tmp_sleep_time;
	float	tick_count;

public:
	Contents();
	~Contents();

	void init_contents();
	void init_sector();
	void init_buildings();
	void init_sims();
	void init_colliders_inform();

	void process_packet(int user_id, char* buf);

	void enter_game(int user_id, char name[]);
	void do_move(int user_id, float xVel, float zVel, float rotAngle, float run_level);
	void disconnect(int user_id);
	void login_fail(int user_id);

	void do_construct(int user_id, int b_type, int b_name, float xpos, float zpos, float angle);
	void do_destruct(int user_id);
	void destruct_all(int user_id);

	void update();
	void update_sim();

	pair<int, int> calculate_sector_num(float xPos, float zPos);
};