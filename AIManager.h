#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*�� ������ private ������ �����ϼ���.*/:
	int simIndex = 0;
	float time = 0.0f;

public  /*�� ������ public ������ �����ϼ���.*/:
	static AIManager* Instance;

	map<int, Sim*> sims;
	std::vector<Village*> villages;

private:
	friend class GameObject;
	friend class MonoBehavior<AIManager>;
	AIManager() = default;
	AIManager(AIManager&) = default;

public:
	~AIManager() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		Time::deltaTime = 0;
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		time += Time::deltaTime;
		
		if (time > 100.f)
		{
			for (auto s : AIManager::Instance->sims)
			{
				Messenger->CreateMessage(0, s.first, s.first, Msg_Sleep);
			}
			time -= 100.f;
		}

		Messenger->Timer();
	}

	int AddSim(Sim* sim)
	{
		sim->id = simIndex;
		sims[simIndex++] = sim;

		sim->stateMachine.PushState(IdleState::Instance());
		sim->stateMachine.GetCurrentState()->Enter(sim);

		return simIndex;
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};