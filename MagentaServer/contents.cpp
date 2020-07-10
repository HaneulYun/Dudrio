#include "contents.h"

Contents::Contents()
{
	logic_run = false;
}

Contents::~Contents()
{
	stop_contents();
}

void Contents::start_contents()
{
	logic_run = true;
	logic_thread = thread([this]() { logic_thread_loop(); });
}

void Contents::stop_contents()
{
	logic_run = false;
	if (logic_thread.joinable())
		logic_thread.join();
}

void Contents::logic_thread_loop()
{
	while (logic_run){
		if (!recvQueue.empty()){

		}
	}
}

void Contents::add_packet()
{

}