#pragma once

#define NUM_OF_CPU 1

class IOCPServer {
private:
	HANDLE	g_iocp;
	SOCKET	l_socket;

	vector<thread>	worker_threads;
	bool			worker_run;
	thread			accept_thread;
	bool			accept_run;

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
	void send_disconnect_packet(int user_id);

	// recv -----------------------------------
	void recv_packet_construct(int user_id, int io_byte);
	void process_packet(int user_id, char* buf);

	void make_room(int user_id, SOCKADDR_IN& sock);
	void delete_room(int user_id);
	void disconnect_server(int user_id);
	void disconnect_client(int user_id);
};