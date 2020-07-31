#pragma once

constexpr int MAX_ID_LEN = 16;
constexpr int MAX_SERVER = 100;
constexpr int MAX_USER = 10000;

#define SERVER_TO_LOBBY_SERVER_PORT 9004
#define CLIENT_TO_LOBBY_SERVER_PORT 9008

#define S2LS_LOGIN			20
#define S2LS_LOGOUT			21

#define LS2S_LOGIN_OK		25
#define LS2S_LOGIN_FAIL		26

#define C2LS_LOGIN_HOST		30
#define C2LS_LOGIN_GUEST	31

#define LS2C_LOGIN_OK_HOST	35
#define LS2C_LOGIN_OK_GUEST 36

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

// Client to Lobby server ----------
struct c2ls_packet_login_host {
	unsigned char size;
	char type;
	char name[MAX_ID_LEN + 1];
	int terrain_size;
	int frequency;
	int octaves;
	int seed;
};

struct c2ls_packet_login_guest {
	unsigned char size;
	char type;
	char name[MAX_ID_LEN + 1];
};

// Lobby server to Client ----------
struct ls2c_packet_login_ok_host {
	unsigned char size;
	char type;
	int server_port;
	char* serverIP;
};

struct ls2c_packet_login_ok_guest {
	unsigned char size;
	char type;
	int server_port;
	char* serverIP;
	int terrain_size;
	int frequency;
	int octaves;
	int seed;
};

#pragma pack (pop)