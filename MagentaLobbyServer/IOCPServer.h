#pragma once

#define NUM_OF_CPU 1

class IOCPServer {
private:
	HANDLE	g_iocp;
	SOCKET	l_socket;

	HANDLE	g_ciocp;
	SOCKET	cl_socket;

	vector<thread>	server_worker_threads;
	bool			server_worker_run;
	thread			server_accept_thread;
	bool			server_accept_run;
	vector<thread>	client_worker_threads;
	bool			client_worker_run;
	thread			client_accept_thread;
	bool			client_accept_run;

public: 
	IOCPServer();
	~IOCPServer();
	void init_server();
	void start_server();
	void init_clients();

	// getter ---------------------------------
	HANDLE& get_iocp_handle() { return g_iocp; }

	// thread ---------------------------------
	void destroy_threads();
	void create_worker_threads();
	void create_accept_threads();
	void server_worker_thread_loop();
	void server_accept_thread_loop();
	void client_worker_thread_loop();
	void client_accept_thread_loop();

	// send -----------------------------------
	void send_packet_server(int user_id, void* p);
	void send_packet_client(int user_id, void* p);
	void send_login_ok_packet(int user_id);
	void send_login_fail_packet(int user_id);
	void send_disconnect_packet(int user_id);

	void send_login_ok_host_packet(int user_id, int room_id);
	void send_login_ok_guest_packet(int user_id);
	void send_login_fail_host_packet(int user_id);
	void send_disconnect_client_packet(int user_id);
	void send_new_room_packet(int user_id, int room_id);
	void send_delete_room_packet(int user_id, int room_id);

	// recv -----------------------------------
	void server_recv_packet_construct(int user_id, int io_byte);
	void client_recv_packet_construct(int user_id, int io_byte);
	void process_packet(int user_id, char* buf);

	void make_room(int user_id, SOCKADDR_IN& sock);
	void delete_room(int user_id);
	void disconnect_server(int user_id);
	void disconnect_client(int user_id);
};