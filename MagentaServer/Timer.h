#pragma once
#include "main.h"

enum EventType {};

struct timer_event
{
	int			obj_id;
	EventType	event_id;
	high_resolution_clock::time_point	wakeup_time;
	int			target_id;

	constexpr bool operator <(const timer_event& left) const
	{
		return (wakeup_time > left.wakeup_time);
	}
};

class Timer
{
private:
	HANDLE	g_iocp;
	thread	timer_thread;
	RWLock	timer_lock;
	priority_queue<timer_event>	timer_queue;
	bool	timer_run;

private:
	void timer_thread_loop();

public:
	Timer();
	~Timer();
	void add_event(timer_event ev);
	void start_timer(HANDLE& h_iocp);
	void stop_timer();
};