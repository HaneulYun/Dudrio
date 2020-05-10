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
#define C2S_DESTRUCT		5	// Host to Server
#define C2S_DESTRUCT_ALL	6	// Host to Server

#define S2C_LOGIN_OK		1
#define S2C_MOVE			2
#define S2C_ENTER			3
#define S2C_LEAVE			4
#define S2C_CONSTRUCT		5	// Server to Guest
#define S2C_DESTRUCT		6	// Server to Guest
#define S2C_DESTRUCT_ALL	7	// Server to Guest

#pragma pack(push ,1)

enum class BuildingType
{
	/*LandMark*/
	Well_01 = 0,

	/*House*/
	House_01 = 100, House_02,

	/*Theme*/

	/*landscape*/
	Tree_01 = 300, Tree_02, Tree_03, Tree_04, Tree_05,
	Tree_Stump_01, Tree_Stump_02,
	Plant_01, Flower_01, Flower_02, Flower_03, Flower_04, Flower_05, Flower_06,
	Grass_01, Grass_02, Grass_03,
	Mushroom_01, Mushroom_02, Mushroom_03, Mushroom_04, Mushroom_05, Mushroom_06,
	Stone_Big_01, Stone_Big_02, Stone_Big_03, Stone_Big_04, Stone_Big_05, Stone_Big_06,
	Stone_Medium_01, Stone_Medium_02, Stone_Medium_03, Stone_Medium_04, Stone_Medium_05,
	Stone_Small_01, Stone_Small_02, Stone_Small_03, Stone_Small_04, Stone_Small_05,
	Stone_Flat_01, Stone_Flat_02, Stone_Flat_03, Stone_Flat_04, Stone_Flat_05,
	StonePath_01, StonePath_02, StonePath_03,

	/*Decoration*/
	Fence_01 = 400, Fence_02, Fence_03, Fence_04,
	Street_Light_01,
	Bucket, Barrel, Pitchfork, Axe, Ladder, Spike, Cart_01, Cart_02,
	Torch_01, Torch_02,
	Logs_01, Logs_02, Log_01, Log_02, Log_03, Log_04,
	Pot_01, Pot_02, Pot_03, Pot_04, Pot_05, Pot_06, Cauldron, Crate_Open, Crate_Closed,
	Hay_Small_01, Hay_Small_02, Hay_Stack,
	Apple, Potato, Tomato, Fish, Watermellon,
	Sack_Apple, Sack_Flour, Sack_Potato, Sack_Tomato, Sack_01, Sack_02,
	Pumpkin_01, Pumpkin_02, Pumpkin_03

};


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
	BuildingType buildingType;
	float xPos, yPos, zPos;
	float rotAngle;

	bool operator== (const BuildingInform& b) const
	{
		return ((xPos == b.xPos) && (yPos == b.yPos)
			&& (zPos == b.zPos));
	}

	friend std::ostream& operator<<(std::ostream& os, const BuildingInform& b);
	friend std::istream& operator>>(std::istream& in, BuildingInform& b);
};

struct BuildingInformHasher
{
	std::size_t operator()(const BuildingInform& b) const
	{
		using std::size_t;
		using std::hash;

		return ((hash<float>()(b.xPos)
			^ (hash<float>()(b.yPos) << 1)) >> 1) ^ (hash<float>()(b.zPos) << 1);
	}
};

static std::ostream& operator<<(std::ostream& os, const BuildingInform& b)
{
	os << static_cast<int>(b.buildingType) << " " << b.xPos << " " << b.yPos << " " << b.zPos << " " << b.rotAngle << std::endl;
	return os;
}

static std::istream& operator>>(std::istream& in, BuildingInform& b)
{
	int input;
	in >> input >> b.xPos >> b.yPos >> b.zPos >> b.rotAngle;
	b.buildingType = static_cast<BuildingType>(input);
	return in;
}

struct sc_packet_construct
{
	char size;
	char type;
	int id;
	BuildingInform b_inform;
};

struct sc_packet_destruct
{
	char size;
	char type;
	int id;
	BuildingInform b_inform;
};

struct sc_packet_destruct_all
{
	char size;
	char type;
	int id;
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
	float x, z;
	float xMove, zMove;
};

struct cs_packet_construct
{
	char size;
	char type;

	BuildingInform b_inform;
};

struct cs_packet_destruct
{
	char size;
	char type;

	BuildingInform b_inform;
};

struct cs_packet_destruct_all
{
	char size;
	char type;
	int id;
};

#pragma pack (pop)