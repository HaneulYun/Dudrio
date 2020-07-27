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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		Time::deltaTime = 0;
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		stateMachine.Update();
	}

	bool HandleMessage(const Telegram& msg)
	{
		if (msg.msg == Msg_Build)
			buildInfo = *static_cast<BuildMessageInfo*>(msg.extraInfo);

		return stateMachine.HandleMessage(msg);
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};