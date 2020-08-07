#pragma once

#define NUM_OF_CPU 4

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
	int				server_port;

public:
	IOCPServer();
	~IOCPServer();
	void init_server();
	void start_server();
	void init_clients();

	// getter ---------------------------------
	HANDLE& get_iocp_handle() { return g_iocp; }

	// thread ---------------------------------
	void create_worker_threads();
	void create_accept_threads();
	void destroy_threads();
	void worker_thread_loop();
	void accept_thread_loop();

	// send -----------------------------------
	void send_packet(int user_id, void* p);
	void send_login_ok_packet(int user_id);
	void send_login_fail_packet(int user_id);

	void send_enter_packet(int user_id, int o_id);
	void send_leave_packet(int user_id, int o_id);
	void send_move_packet(int user_id, int mover, float dAngle);	// move + rotate

	void send_construct_packet(int user_id, int type, int name, float x, float z, float angle, int range);
	void send_destruct_packet(int user_id, int type, int name, float x, float z);
	void send_destruct_all_packet(int user_id);

	void send_enter_sim_packet(int user_id, int o_id);
	void send_leave_sim_packet(int user_id, int o_id);
	void send_move_sim_packet(int user_id, int mover, float dAngle);	// move + rotate

	void send_game_time_packet(int user_id);
	void send_chat_packet(int user_id, int chatter, wchar_t mess[]);
	void send_teleport_packet(int user_id, int mover);

	// recv -----------------------------------
	void recv_packet_construct(int user_id, int io_byte);

};