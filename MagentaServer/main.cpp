#include "main.h"

class IOCPServer iocp;
class Timer	timer;
class Contents contents;

int main()
{
	iocp.init_server();
	iocp.start_server();
	contents.start_contents();
	timer.start_timer(iocp.get_iocp_handle());

	getchar();
	return 0;
}