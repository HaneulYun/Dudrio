#pragma once

class Client 
{
public:
	mutex	m_cl;
	SOCKET	m_s;
	int		m_id;
	EXOVER	m_recv_over;
	int		m_prev_size;
	char	m_packet_buf[MAX_PACKET_SIZE];
	atomic	<C_STATUS> m_status;
	char	m_name[L_MAX_ID_LEN + 1];
// getpeername으로 소켓 가지고 ip알아낼 수 있음

public:
	Client() {}
	Client(SOCKET& sock, int id): m_s(sock), m_id(id){}
	Client(int id):m_id(id) {}
	~Client() {}
};