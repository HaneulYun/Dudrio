#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class Sim : public MonoBehavior<Sim>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	int id;
	float speed = 4.f;

	GameObject* home;

	StateMachine<Sim> stateMachine;

	deque<Vector2> targetPos;
	deque<Vector2> path;
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
		stateMachine.Update();
	}

	bool HandleMessage(const Telegram& msg)
	{
		return stateMachine.HandleMessage(msg);
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};