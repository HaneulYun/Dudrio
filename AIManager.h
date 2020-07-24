#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	int simIndex = 0;
	float time = 0.0f;

public  /*이 영역에 public 변수를 선언하세요.*/:
	static AIManager* Instance;

	std::map<int, Sim*> sims;
	std::vector<Village*> villages;

	GameObject* simPrefab;

private:
	friend class GameObject;
	friend class MonoBehavior<AIManager>;
	AIManager() = default;
	AIManager(AIManager&) = default;

public:
	~AIManager() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		Time::deltaTime = 0;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		time += Time::deltaTime;
		
		if (time > 60.f)
		{
			for (auto s : AIManager::Instance->sims)
			{
				//Messenger->CreateMessage(0, s.first, s.first, Msg_Sleep);
			}
			time -= 60.f;
		}

		Messenger->Timer();
	}

	int AddSim(GameObject* house, GameObject* landMark)
	{
		GameObject* sim = Scene::scene->Duplicate(simPrefab);
		sim->transform->position = house->transform->position;

		auto simComponent = sim->AddComponent<Sim>();
		simComponent->animator = sim->children[0]->GetComponent<Animator>();
		simComponent->home = house;
		simComponent->id = simIndex;
		sims[simIndex++] = simComponent;

		simComponent->stateMachine.PushState(IdleState::Instance());
		simComponent->stateMachine.GetCurrentState()->Enter(simComponent);

		landMark->GetComponent<Village>()->sims.push_back(sim);

		return simIndex;
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};