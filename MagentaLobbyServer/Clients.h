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
	char	m_name[MAX_ID_LEN + 1];
// getpeername으로 소켓 가지고 ip알아낼 수 있음

public:
	Client(SOCKET& sock, int id);
	Client(int id);
	~Client();
};