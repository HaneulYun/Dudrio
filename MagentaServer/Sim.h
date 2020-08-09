#pragma once
#include "StateMachine.h"
#include "Clients.h"

struct TargetInfo
{
	Vector2D pos;
	float posOffset = 0;

	TargetInfo(Vector2D pos, float posOffset) : pos(pos), posOffset(posOffset) {}
	TargetInfo() = default;
};

class Sim
{
public:
	int id;
	char appearance;
	Vector2D pos;
	Vector2D forward;
	float rotAngle;

	Building* home;

	StateMachine<Sim> stateMachine;
	deque<TargetInfo> targetPos;
	deque<Vector2D> path;
	BuildMessageInfo buildInfo;
	vector<int>	prevNearClients;

public:
	Sim() {}
	Sim(int idx, float x, float z) :id(idx), pos(x, z), forward(0, 1), rotAngle(0) 
	{
		stateMachine.SetOwner(this); 
		int ix = std::floor(x);
		int iz = std::floor(z);
		appearance = (ix + iz) % 21;
	}
	~Sim() {}

	bool is_near(const Client& other);
	void Update();

	bool is_sector_change(float prevX, float prevZ);
	void erase_sim_in_sector(float x, float z);
	void erase_sim_in_sector();
	void insert_client_in_sector();

	vector<int> get_near_clients();

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};