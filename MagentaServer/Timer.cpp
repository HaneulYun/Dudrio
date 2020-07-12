#include "Timer.h"

Timer::Timer()
{
	timer_run = false;
}

Timer::~Timer()
{
	stop_timer();
}

void Timer::start_timer(HANDLE& h_iocp)
{
	g_iocp = h_iocp;
	timer_run = true;
	timer_thread = thread([this]() { timer_thread_loop(); });
}

void Timer::stop_timer()
{
	timer_run = false;
	if (timer_thread.joinable())
		timer_thread.join();
}

void Timer::timer_thread_loop()
{
	while (timer_run){
		if (!timer_queue.empty()) {
			timer_lock.EnterReadLock();
			timer_event ev = timer_queue.top();
			if (ev.wakeup_time > high_resolution_clock::now()) {
				timer_lock.LeaveReadLock();
				continue;
			}
			timer_lock.LeaveReadLock();
			timer_lock.EnterWriteLock();
			timer_queue.pop();
			timer_lock.LeaveWriteLock();

			EXOVER* over = new EXOVER;
			switch (ev.event_id)
			{
			case 1:
			{
				//over->op = ev.event_id;
				//over->p_id = ev.target_id;
			}
			break;
			default:
			{
				cout << "Timer_thread : INVALID EVENT ID !!" << endl;
				continue;
			}
			break;
			}
			PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
		}
		else
			this_thread::sleep_for(milliseconds(5));
	}
}

void Timer::add_event(timer_event ev)
{
	timer_lock.EnterWriteLock();
	timer_queue.push(ev);
	timer_lock.LeaveWriteLock();
}
