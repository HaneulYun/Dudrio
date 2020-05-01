#pragma once

constexpr int MAX_ID_LEN = 50;
constexpr int MAX_STR_LEN = 255;
constexpr int MAX_USER = 10;

#define WORLD_WIDTH		8
#define WORLD_HEIGHT	8

#define SERVER_PORT		9000

#define C2S_LOGIN			1
#define C2S_MOVE			2
#define C2S_LOGIN_HOST		3	// Host to Server
#define C2S_CONSTRUCT		4	// Host to Server

#define S2C_LOGIN_OK		1
#define S2C_MOVE			2
#define S2C_ENTER			3
#define S2C_LEAVE			4
#define S2C_CONSTRUCT		5	

#pragma pack(push ,1)

struct sc_packet_login_ok {
	char size;
	char type;
	int id;
	float x, z;
	float xMove, zMove;
	short hp;
	short level;
	int	exp;
};

struct sc_packet_move {
	char size;
	char type;
	int id;
	float x, z;
	float xMove, zMove;
};

constexpr unsigned char B_SPHERE = 0;
constexpr unsigned char B_CUBE = 1;

struct BuildingInform
{
	char buildingType;
	float xPos, yPos, zPos;
	float rotAngle;
};

struct sc_packet_construct
{
	char size;
	char type;
	int id;
	BuildingInform b_inform;
};

constexpr unsigned char O_GUEST = 0;
constexpr unsigned char O_HOST = 1;

struct sc_packet_enter {
	char size;
	char type;
	int id;
	char name[MAX_ID_LEN];
	char o_type;
	float x, z;
	float xMove, zMove;
};

struct sc_packet_leave {
	char size;
	char type;
	int id;
};

struct cs_packet_login {
	char	size;
	char	type;
	char	name[MAX_ID_LEN];
};

struct cs_packet_move {
	char	size;
	char	type;

	float xMove, zMove;
};

struct cs_packet_construct
{
	char size;
	char type;

	BuildingInform b_inform;
};

#pragma pack (pop)