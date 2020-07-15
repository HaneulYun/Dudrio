#pragma once
#include "main.h"

class Contents
{
private:
	queue<pair<int, char*>> recvQueue;
	thread		logic_thread;
	RWLock		logic_lock;
	bool		logic_run;

public:
	int		host_id;

private:
	void logic_thread_loop();

public:
	Contents();
	~Contents();

	void start_contents();
	void stop_contents();

	void add_packet(int user_id, char* buf);

	void disconnect(int user_id);

	void do_construct(int user_id, BuildingInform b_inform);
	void do_destruct(int user_id, BuildingInform b_inform);
	void destruct_all(int user_id);
};