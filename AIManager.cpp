#include "pch.h"
#include "AIManager.h"


void AIManager::Start(/*초기화 코드를 작성하세요.*/)
{

}

void AIManager::Update(/*업데이트 코드를 작성하세요.*/)
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

int AIManager::AddSim(GameObject* landmark, GameObject* house)
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