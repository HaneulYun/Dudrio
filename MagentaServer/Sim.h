#pragma once
#include "main.h"

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

	Vector2D pos;
	Vector2D forward;
	float rotAngle;

	Building* home;

	StateMachine<Sim> stateMachine;
	deque<TargetInfo> targetPos;
	deque<Vector2D> path;
	BuildMessageInfo buildInfo;

public:
	~Sim() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		Time::deltaTime = 0;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		stateMachine.Update();
	}

	bool HandleMessage(const Telegram& msg)
	{
		if (msg.msg == Msg_Build)
			buildInfo = *static_cast<BuildMessageInfo*>(msg.extraInfo);

		return stateMachine.HandleMessage(msg);
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};