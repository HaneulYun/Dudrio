#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*�� ������ private ������ �����ϼ���.*/:
	int simIndex = 0;
	float time = 0.0f;

public  /*�� ������ public ������ �����ϼ���.*/:
	static AIManager* Instance;

	std::map<int, Sim*> simList;
	std::vector<Village*> villages;

	GameObject* simPrefab;

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
		
		if (time > 60.f)
		{
			for (auto s : simList)
			{
				Messenger->CreateMessage(0, s.first, s.first, Msg_Sleep);
			}
			time -= 60.f;
		}

		Messenger->Timer();
	}

	int AddSim(GameObject* landmark, GameObject* house)
	{
		GameObject* sim = Scene::scene->Duplicate(simPrefab);
		sim->transform->position = house->transform->position;

		auto simComponent = sim->AddComponent<Sim>();
		simComponent->animator = sim->children[0]->GetComponent<Animator>();
		simComponent->home = house;
		simComponent->id = simIndex;
		simList[simIndex++] = simComponent;

		simComponent->stateMachine.PushState(IdleState::Instance());
		simComponent->stateMachine.GetCurrentState()->Enter(simComponent);

		landmark->GetComponent<Village>()->sims[house] = sim;

		return simIndex;
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};