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

	Client(SOCKET& sock, int id): m_s(sock), m_id(id)
	{
		m_prev_size = 0;
		m_recv_over.op = OP_RECV;
		ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
		m_recv_over.wsabuf.buf = m_recv_over.io_buf;
		m_recv_over.wsabuf.len = MAX_BUF_SIZE;
		m_status = ST_FREE;
	}
	Client(int id):m_id(id)
	{
		m_prev_size = 0;
		m_recv_over.op = OP_RECV;
		ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over));
		m_recv_over.wsabuf.buf = m_recv_over.io_buf;
		m_recv_over.wsabuf.len = MAX_BUF_SIZE;
		m_status = ST_FREE;
	}
	~Client() {}
};