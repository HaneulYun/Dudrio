#pragma once
#include "main.h"

class Contents
{
private:
	queue<pair<int, char*>> recvQueue;
	//thread		logic_thread;
	vector<thread> logic_threads;
	//RWLock		logic_lock;
	mutex		logic_lock;
	bool		logic_run;

public:
	int		host_id;

private:
	void logic_thread_loop();

public:
	Contents();
	~Contents();

	void init_contents();
	void start_contents();
	void stop_contents();

	void add_packet(int user_id, char* buf);

	void enter_game(int user_id, char name[]);
	void do_move(int user_id, float xVel, float zVel, float rotAngle, float run_level);
	void disconnect(int user_id);
	void login_fail(int user_id);

	void do_construct(int user_id, BuildingInform b_inform);
	void do_destruct(int user_id, BuildingInform b_inform);
	void destruct_all(int user_id);

	pair<int, int> calculate_sector_num(float xPos, float zPos);
};