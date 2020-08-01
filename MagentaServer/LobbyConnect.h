#pragma once

class LobbyServer {
private:
	SOCKET	l_socket;

public:
	LobbyServer();
	~LobbyServer();

	void init_server();

	void send_packet(void* p);
	void send_login_packet();
	void send_logout_packet();
	void send_host_logout_packet();

	void receiver();

	void process_packet(char* ptr);
	void process_data(char* net_buf, size_t io_byte);
	int connect_nonblock(SOCKET sockfd, const struct sockaddr FAR* name, int namelen, int timeout);
};