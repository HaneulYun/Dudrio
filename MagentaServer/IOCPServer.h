#pragma once
#include "main.h"

#define NUM_OF_CPU 6

class IOCPServer {
private:
	HANDLE	g_iocp;
	SOCKET	l_socket;

	high_resolution_clock::time_point server_time;
	vector<thread>	worker_threads;
	bool			worker_run;
	thread			accept_thread;
	bool			accept_run;

public:
	IOCPServer();
	~IOCPServer();
	void init_server();
	void start_server();

	// getter ---------------------------------
	HANDLE& get_iocp_handle() { return g_iocp; }

	// thread ---------------------------------
	void create_threads();
	void destroy_threads();
	void worker_thread_loop();
	void accept_thread_loop();

	// send -----------------------------------
	void send_packet(int user_id, void* p);
	void send_login_ok_packet(int user_id);
	void send_login_fail_packet(int user_id);

	void send_enter_packet(int user_id, int o_id);
	void send_leave_packet(int user_id, int o_id);
	void send_move_packet(int user_id, int mover);	// move + rotate

	void send_construct_packet(int user_id, BuildingInform b_inform);
	void send_destruct_packet(int user_id, BuildingInform b_inform);
	void send_destruct_all_packet(int user_id);

	// recv -----------------------------------
	void process_packet(int user_id, char* buf);
	void recv_packet_construct(int user_id, int io_byte);

};