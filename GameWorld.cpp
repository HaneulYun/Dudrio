#include "pch.h"
#include "GameWorld.h"



void GameWorld::Start(/*초기화 코드를 작성하세요.*/)
{
	gameWorld = this;
}

void GameWorld::Update(/*업데이트 코드를 작성하세요.*/)
{
	gameTimeUpdate();
	aiUpdate();

	for (auto landmark : buildingList)
	{
		for (auto sim : landmark.first->GetComponent<Village>()->simList)
		{

		}
		for (auto house : landmark.second[House])
		{

		}
		for (auto light : landmark.second[Lighting])
		{
		}
	}
}

void GameWorld::aiUpdate()
{
	if (!simList.empty())
		AIManager::aiManager->aiUpdate();
}

void GameWorld::gameTimeUpdate()
{
	gameTime += Time::deltaTime * timeSpeed;
	gameDeltaTime = Time::deltaTime * timeSpeed;
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
	simList[simIndex++] = sim;

	simComponent->stateMachine.PushState(IdleState::Instance());
	simComponent->stateMachine.GetCurrentState()->Enter(simComponent);

	landmark->GetComponent<Village>()->simList[house] = sim;

	return simIndex;
}

int GameWorld::eraseSim(GameObject* landmark, GameObject* house)
{
	GameObject* sim = landmark->GetComponent<Village>()->simList[house];

	int id = sim->GetComponent<Sim>()->id;

	landmark->GetComponent<Village>()->simList.erase(house);
	simList.erase(id);

	return id;
}