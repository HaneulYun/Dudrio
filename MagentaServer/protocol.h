#pragma once

constexpr int MAX_ID_LEN = 16;
constexpr int MAX_STR_LEN = 100 - MAX_ID_LEN + 1;
constexpr int MAX_USER = 10000;

#define WORLD_WIDTH		1000
#define WORLD_HEIGHT	1000

#define SERVER_PORT		9000

#define C2S_LOGIN_GUEST		1	// Guest to Server
#define C2S_LOGIN_HOST		2	// Host to Server
#define C2S_LOGOUT			3
#define C2S_MOVE_START		4	// Guest to Server
#define C2S_MOVE			5	// Guest to Server
#define C2S_CONSTRUCT		6	// Host to Server
#define C2S_DESTRUCT		7	// Host to Server
#define C2S_DESTRUCT_ALL	8	// Host to Server
#define C2S_CHAT			9

#define S2C_LOGIN_OK		1
#define S2C_LOGIN_FAIL		2
#define S2C_ENTER			3
#define S2C_LEAVE			4
#define S2C_MOVE			5
#define S2C_CONSTRUCT		6	// Server to Guest
#define S2C_DESTRUCT		7	// Server to Guest
#define S2C_DESTRUCT_ALL	8	// Server to Guest
#define S2C_CHAT			9
#define S2C_SIM_ENTER		10
#define S2C_SIM_LEAVE		11
#define S2C_SIM_MOVE		12
#define S2C_GAME_TIME		13

#pragma pack(push ,1)

///////////////////////////////////////////////
// SERVER TO CLIENT
///////////////////////////////////////////////
// LOGIN ---------------------
struct sc_packet_login_ok {
	unsigned char size;
	char type;
	int id;
	int host_id;
	char host_name[MAX_ID_LEN + 1];
	float game_time;
	float xPos, zPos;
	float xVel, zVel;
	float rotAngle;
	float terrainSize;
	float frequency;
	int octaves;
	int seed;
};

struct sc_packet_login_fail {
	unsigned char size;
	char type;
};

struct sc_packet_game_time {
	unsigned char size;
	char type;
	float game_time;
};

struct sc_packet_chat {
	unsigned char size;
	char type;
	int	 id;
	wchar_t mess[MAX_STR_LEN];
};

// PLAYER ----------------------
constexpr unsigned char O_GUEST = 0;
constexpr unsigned char O_HOST = 1;

struct sc_packet_enter {
	unsigned char size;
	char type;
	int id;
	char name[MAX_ID_LEN + 1];
	char o_type;
	float xPos, zPos;
	float xVel, zVel;
	float rotAngle;
};

struct sc_packet_leave {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_move {
	unsigned char size;
	char type;
	int id;
	float xPos, zPos;
	float xVel, zVel;
	float rotAngle;
	unsigned move_time;
};

// SIM -------------------------
struct sc_packet_sim_enter {
	unsigned char size;
	char type;
	int id;
	float xPos, zPos;
	float xVel, zVel;
	float rotAngle;
};

struct sc_packet_sim_leave {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_sim_move {
	unsigned char size;
	char type;
	int id;
	float xPos, zPos;
	float xVel, zVel;
	float rotAngle;
};

// CONSTRUCT -------------------
struct sc_packet_construct
{
	unsigned char size;
	char type;
	int sim_id;
	int building_type;
	int building_name;
	float xPos, zPos;
	float angle;
};

struct sc_packet_destruct
{
	unsigned char size;
	char type;

	// ºôµù Á¤º¸µé
};

struct sc_packet_destruct_all
{
	unsigned char size;
	char type;
};

///////////////////////////////////////////////
// CLIENT TO SERVER
///////////////////////////////////////////////
// LOGIN------------------------
struct cs_packet_login_guest {
	unsigned char	size;
	char	type;
	char	name[MAX_ID_LEN + 1];
};

struct cs_packet_login_host {
	unsigned char	size;
	char	type;
	char	name[MAX_ID_LEN + 1];
	float	terrainSize;
	float	frequency;
	int		octaves;
	int		seed;
	float	game_time;
};

struct cs_packet_logout {
	unsigned char size;
	char type;
};

struct cs_packet_chat {
	unsigned char size;
	char type;
	wchar_t message[MAX_STR_LEN];
};

// PLAYER ------------------------
struct cs_packet_move_start {
	unsigned char size;
	char type;
	float xVel, zVel;
	float rotAngle;
	float run_level;
	unsigned move_time;
};

struct cs_packet_move {
	unsigned char size;
	char type;
	float xVel, zVel;
	float rotAngle;
	float run_level;
	unsigned move_time;
};

// CONSTRUCT ---------------------
struct cs_packet_construct{
	unsigned char size;
	char type;
	int building_type;
	int building_name;
	float xpos, zpos;
	float angle;
};

struct cs_packet_destruct{
	unsigned char size;
	char type;

	// ºôµù Á¤º¸µé
};

struct cs_packet_destruct_all{
	unsigned char size;
	char type;
};
#pragma pack (pop)