#pragma once

class Contents
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Lighting, Fence, Prop, Count, None = -1 };
	vector<struct Collider> collider_info[BuildingType::Count];

	int		host_id;
	int		sim_index;
	float	ingame_time;
	float	tick_count;

private:
	float	server_time;

	float	second = 1000.f;
	float	max_oneday = 60 * 15;	// 하루
	float	dawn_start_time = 6 * 37.5f;	// 새벽 시작 시간
	float	day_start_time = 10 * 37.5f;	// 낮의 시작 시간
	float	night_start_time = 21 * 37.5f;	// 밤의 시작 시간

	bool	sleep_flag = false;
	bool	wakeup_flag = false;

	bool	send_packet_flag[4]{ false };	// 0시, 새벽, 낮, 밤
public:
	Contents();
	~Contents();

	void init_contents();
	void init_sector();
	void init_buildings();
	void init_sims();
	void init_colliders_inform();

	void process_packet(int user_id, char* buf);

	void login_fail(int user_id);
	void enter_game(int user_id, wchar_t name[]);
	void disconnect(int user_id);
	void chatting(int user_id, wchar_t mess[]);
	void do_move(int user_id, float xVel, float zVel, float rotAngle, float run_level);

	void do_construct(int user_id, int b_type, int b_name, float xpos, float zpos, float angle, int land_range);
	void do_destruct(int user_id, int b_type, int b_name, float xpos, float zpos, float angle);
	void destruct_all(int user_id);

	void update();
	void update_sim();

	class Village* get_my_landmark(class Building* b);
	pair<int, int> calculate_sector_num(float xPos, float zPos);
};