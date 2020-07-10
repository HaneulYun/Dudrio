#pragma once
#include "main.h"

class Contents
{
private:
	std::queue<packet> recvQueue;
	std::thread logic_thread;
	RWLock		logic_lock;
	bool		logic_run;

private:
	void logic_thread_loop();

public:
	Contents();
	~Contents();

	void start_contents();
	void stop_contents();

	void add_packet();
};