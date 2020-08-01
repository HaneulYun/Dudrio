#include "pch.h"
#include "LobbyConnect.h"

#define BUFSIZE 200

LobbyServer::LobbyServer()
{
}

LobbyServer::~LobbyServer()
{
	send_logout_packet();
	WSACleanup();
}

void LobbyServer::init_server()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);

	SOCKADDR_IN serveraddr{};
	memset(&serveraddr, 0, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_TO_LOBBY_SERVER_PORT);
	serveraddr.sin_addr.S_un.S_addr = inet_addr(LOBBY_SERVER_IP);

	int retval = connect_nonblock(l_socket, (struct sockaddr*) & serveraddr, sizeof(serveraddr), 5);
	if (retval == SOCKET_ERROR) {
		cout << "Connect error! " << WSAGetLastError() << endl;
		closesocket(l_socket);
		WSACleanup();
		return init_server();
	}
	else if (retval == 0) {
		unsigned long on = true;
		int nRet = ioctlsocket(l_socket, FIONBIO, &on);
		send_login_packet();
		receiver();
	}
	else
		return init_server();
}

void LobbyServer::send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);
	int send_bytes = send(l_socket, p, (unsigned char)p[0], 0);
	if (send_bytes > 0)
		cout << "Send complete" << endl;
}

void LobbyServer::send_login_packet()
{
	s2ls_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = S2LS_LOGIN;

	std::cout << "Send Login packet to Lobby server" << endl;
	send_packet(&l_packet);
}

void LobbyServer::send_logout_packet()
{
	cs_packet_logout l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = S2LS_LOGOUT;

	send_packet(&l_packet);

	std::cout << "Send Logout packet to Lobby server" << endl;
	closesocket(l_socket);
}

void LobbyServer::send_host_logout_packet()
{
	s2ls_packet_host_logout l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = S2LS_HOST_LOGOUT;

	std::cout << "Send Host Logout packet to Lobby server" << endl;
	send_packet(&l_packet);
}

void LobbyServer::receiver()
{
	while (1){
		char net_buf[BUFSIZE];
		auto retval = recv(l_socket, net_buf, BUFSIZE, 0);

		if (retval > 0)	process_data(net_buf, retval);
	}
}

void LobbyServer::process_packet(char* ptr)
{
	switch (ptr[1])
	{
	case LS2S_LOGIN_OK:
	{
		ls2s_packet_login_ok* my_packet = reinterpret_cast<ls2s_packet_login_ok*>(ptr);
		iocp.server_port = my_packet->server_port;
		iocp.init_server();
		iocp.start_server();
		contents.init_contents();
		timer.start_timer(iocp.get_iocp_handle());
	}
		break;
	case LS2S_LOGIN_FAIL:
	{
		ls2s_packet_login_fail* my_packet = reinterpret_cast<ls2s_packet_login_fail*>(ptr);
		closesocket(l_socket);
	}
	break;
	case LS2S_DISCONNECT:
	{
		ls2s_packet_disconnect* my_packet = reinterpret_cast<ls2s_packet_disconnect*>(ptr);
		iocp.destroy_threads();
		timer.stop_timer();
		contents.init_contents();
		closesocket(l_socket);
	}
	break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void LobbyServer::process_data(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUFSIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = (unsigned char)ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			process_packet(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}

int LobbyServer::connect_nonblock(SOCKET sockfd, const struct sockaddr FAR* name, int namelen, int timeout)
{
	unsigned long nonzero = 1;
	unsigned long zero = 0;
	fd_set rset, wset;
	struct timeval tval;
	int n;
	int nfds = 1;

	if (ioctlsocket(sockfd, FIONBIO, &nonzero) == SOCKET_ERROR)
		return SOCKET_ERROR;

	if ((n = connect(sockfd, (struct sockaddr FAR*)name, namelen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return SOCKET_ERROR;
	}

	if (n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	tval.tv_sec = timeout;
	tval.tv_usec = 0;

	if ((n = select(nfds, &rset, &wset, NULL, timeout ? &tval : NULL)) == 0)
	{
		WSASetLastError(WSAETIMEDOUT);
		return SOCKET_ERROR;
	}
done:
	ioctlsocket(sockfd, FIONBIO, &zero);
	return 0;
}