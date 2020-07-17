#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class Sim : public MonoBehavior<Sim>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
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

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		stateMachine.Update();
	}

	bool HandleMessage(const Telegram& msg)
	{
		return stateMachine.HandleMessage(msg);
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};