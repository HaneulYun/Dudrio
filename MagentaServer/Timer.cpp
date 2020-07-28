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

void Timer::clear_timer()
{
	lock_guard<mutex>lock_guard(timer_lock);
	while (!timer.timer_queue.empty())
		timer_queue.pop();
}

void Timer::timer_thread_loop()
{
	while (timer_run){
		if (!timer_queue.empty()) {
			timer_lock.lock();
			timer_event ev = timer_queue.top();
			if (ev.wakeup_time > high_resolution_clock::now()) {
				timer_lock.unlock();
				continue;
			}
			timer_queue.pop();
			timer_lock.unlock();

			EXOVER* over = new EXOVER;
			switch (ev.event_id)
			{
			case GAME_Update:
			{
				over->op = GAME_UPDATE;
				over->obj_id = ev.obj_id;
				over->target_id = ev.target_id;
			}
			break;
			case SIM_Move:
			{
				over->op = SIM_MOVE;
				over->obj_id = ev.obj_id;
				over->target_id = ev.target_id;
				over->extra_info = ev.extra_info;
			}
			break;
			case SIM_Sleep:
			{
				over->op = SIM_SLEEP;
				over->obj_id = ev.obj_id;
				over->target_id = ev.target_id;
			}
			break;
			case SIM_WakeUp:
			{
				over->op = SIM_WAKEUP;
				over->obj_id = ev.obj_id;
				over->target_id = ev.target_id;
				//over->extra_info = ev.extra_info;
			}
			break;
			case SIM_Build:
			{
				over->op = SIM_BUILD;
				over->obj_id = ev.obj_id;
				over->target_id = ev.target_id;
				over->extra_info = ev.extra_info;
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
			this_thread::sleep_for(milliseconds(2));
	}
}

void Timer::add_event(timer_event ev)
{
	lock_guard<mutex>lock_guard(timer_lock);
	timer_queue.push(ev);
}
