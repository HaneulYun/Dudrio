#pragma once
#include "CyanEngine\CyanEngine\framework.h"

struct TargetInfo
{
	Vector2 pos;
	float posOffset = 0;

	TargetInfo(Vector2 pos, float posOffset) : pos(pos), posOffset(posOffset) {}
	TargetInfo() = default;
};

class Sim : public MonoBehavior<Sim>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	int id;
	float speed = 1.f;

	GameObject* home;

	StateMachine<Sim> stateMachine;
	std::deque<TargetInfo> targetPos;
	std::deque<Vector2> path;
	BuildMessageInfo buildInfo;

	Animator* animator;

private:
	friend class GameObject;
	friend class MonoBehavior<Sim>;
	Sim() : stateMachine(this) {};
	Sim(Sim&) = default;

public:
	~Sim() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		//stateMachine.Update();
	}

	bool HandleMessage(const Telegram& msg)
	{
		if (msg.msg == Msg_Build)
			buildInfo = *static_cast<BuildMessageInfo*>(msg.extraInfo);

		return stateMachine.HandleMessage(msg);
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};