#pragma once

constexpr int L_MAX_ID_LEN = 16;
constexpr int L_MAX_SERVER = 100;
constexpr int L_MAX_USER = 10000;

#define SERVER_TO_LOBBY_SERVER_PORT 9004
#define CLIENT_TO_LOBBY_SERVER_PORT 9008
#define LOBBY_SERVER_IP	"127.0.0.1"
#define INET_ADDRSTRLEN		22

#define S2LS_LOGIN			20
#define S2LS_LOGOUT			21
#define S2LS_HOST_LOGOUT	22

#define LS2S_LOGIN_OK		25
#define LS2S_LOGIN_FAIL		26
#define LS2S_DISCONNECT		27

#define C2LS_LOGIN_HOST		30
#define C2LS_LOGIN_GUEST	31

#define LS2C_LOGIN_OK_HOST	35
#define LS2C_LOGIN_OK_GUEST 36
#define LS2C_LOGIN_FAIL		37
#define LS2C_NEW_ROOM		38
#define LS2C_DELETE_ROOM	39


#pragma pack(push ,1)

// Server to Lobby server ----------
struct s2ls_packet_login {
	unsigned char size;
	char type;
};

struct s2ls_packet_logout {
	unsigned char size;
	char type;
};

struct s2ls_packet_host_logout {
	unsigned char size;
	char type;
};

// Lobby server to Server ----------
struct ls2s_packet_login_ok {
	unsigned char size;
	char type;
	int server_port;
};

struct ls2s_packet_login_fail {
	unsigned char size;
	char type;
};

struct ls2s_packet_disconnect {
	unsigned char size;
	char type;
};

// Client to Lobby server ----------
struct c2ls_packet_login_host {
	unsigned char size;
	char type;
	char name[L_MAX_ID_LEN + 1];
	int terrain_size;
	int frequency;
	int octaves;
	int seed;
};

struct c2ls_packet_login_guest {
	unsigned char size;
	char type;
	char name[L_MAX_ID_LEN + 1];
};

// Lobby server to Client ----------
struct ls2c_packet_login_ok_host {
	unsigned char size;
	char type;
	int server_port;
	char serverIP[INET_ADDRSTRLEN];
};

struct ls2c_packet_login_ok_guest {
	unsigned char size;
	char type;
};

// 호스트에게만 보내는 패킷들
struct ls2c_pakcet_login_fail {
	unsigned char size;
	char type;
};

// 게스트에게만 보내는 패킷들
struct ls2c_pakcet_new_room {
	unsigned char size;
	char type;
	int room_id;
	char host_name[L_MAX_ID_LEN + 1];
	int server_port;
	char serverIP[INET_ADDRSTRLEN];
	int terrain_size;
	int frequency;
	int octaves;
	int seed;
};

struct ls2c_packet_delete_room {
	unsigned char size;
	char type;
	int room_id;
};

#pragma pack (pop)