#include "IOCPServer.h"

IOCPServer::IOCPServer()
{
	worker_run = true;
	accept_run = true;
}

IOCPServer::~IOCPServer()
{
	WSACleanup();
	destroy_threads();
}

void IOCPServer::init_server()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	l_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	// bind -------------------------------------------------------
	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));

	// listen -----------------------------------------------------
	listen(l_socket, SOMAXCONN);
	
	// 네이글 알고리즘 OFF
	int option = TRUE;
	setsockopt(l_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));
}

void IOCPServer::start_server()
{
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NUM_OF_CPU * 2 + 1);

	//initialize_clients();
	cout << " Start_Server Complete" << endl;
}

// thread ---------------------------------
void IOCPServer::create_threads()
{
	worker_threads.reserve(NUM_OF_CPU * 2 + 2);

	for (int i = 0; i < NUM_OF_CPU * 2 + 1; ++i){
		worker_threads.emplace_back([this]() {worker_thread_loop(); });
	}

	cout << "Create Worker_Threads Complete" << endl;

	accept_thread = thread([this]() { accept_thread_loop(); });

	cout << "Create Accept_Thread Complete" << endl;
}

void IOCPServer::destroy_threads()
{
	worker_run = false;
	CloseHandle(g_iocp);
	for (auto& w_t : worker_threads)
		if (w_t.joinable())
			w_t.join();

	accept_run = false;
	closesocket(l_socket);
	if (accept_thread.joinable())
		accept_thread.join();

	contents.stop_contents();
	timer.stop_timer();
}

void IOCPServer::worker_thread_loop()
{
	while (worker_run) {

	}
}

void IOCPServer::accept_thread_loop()
{
	while (accept_run) {
		//CreateIoCompletionPort(reinterpret_cast<HANDLE>(l_socket), g_iocp, 999, 0);
		//SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		//EXOVER accept_over;
		//ZeroMemory(&accept_over.over, sizeof(accept_over.over));
		//accept_over.op = OP_ACCEPT;
		//accept_over.c_socket = c_socket;
		//AcceptEx(l_socket, c_socket, accept_over.io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);	// 클라이언트 접속에 사용할 소켓을 미리 만들어
	}
}