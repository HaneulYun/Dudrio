#include "pch.h"
#include "GameWorld.h"


void GameWorld::aiUpdate()
{
	if (simList.empty())
		return;

	gameTime += Time::deltaTime;

	if (gameTime > 60.f)
	{
		for (auto s : simList)
		{
			Messenger->CreateMessage(0, s.first, s.first, Msg_Sleep);
		}
		gameTime -= 60.f;
	}

	Messenger->Timer();
}

void GameWorld::buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	buildingList[landmark][(BuildingType)type].push_back(building);

	if (type == BuildingType::House)
		addSim(landmark, building);
}

void GameWorld::deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	buildingList[landmark][(BuildingType)type].erase(find(buildingList[landmark][(BuildingType)type].begin(), buildingList[landmark][(BuildingType)type].end(), building));

	if (type == BuildingType::House)
	{
		eraseSim(landmark, building);
	}
}

int GameWorld::addSim(GameObject* landmark, GameObject* house)
{
	GameObject* sim = Scene::scene->Duplicate(simPrefab);
	sim->transform->position = house->transform->position;

	auto simComponent = sim->AddComponent<Sim>();
	simComponent->animator = sim->children[0]->GetComponent<Animator>();
	simComponent->home = house;
	simComponent->id = simIndex;
	simList[simIndex++] = simComponent;

	// 네트워크 연결 안되어있으면
	simComponent->stateMachine.PushState(IdleState::Instance());
	simComponent->stateMachine.GetCurrentState()->Enter(simComponent);

	landmark->GetComponent<Village>()->sims[house] = sim;

	return simIndex;
}

int GameWorld::eraseSim(GameObject* landmark, GameObject* house)
{
	GameObject* sim = landmark->GetComponent<Village>()->sims[house];

	int id = sim->GetComponent<Sim>()->id;

	landmark->GetComponent<Village>()->sims.erase(house);
	simList.erase(id);

	return id;
}