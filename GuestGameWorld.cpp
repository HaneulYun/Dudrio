#include "pch.h"
#include "GuestGameWorld.h"

void GuestGameWorld::Start(/*초기화 코드를 작성하세요.*/)
{
	GameWorld::Start();
	gameWorld = this;
}

void GuestGameWorld::Update(/*업데이트 코드를 작성하세요.*/)
{
	gameTimeUpdate();

	//for (auto landmark : buildingList)
	//{
	//	for (auto sim : landmark.first->GetComponent<Village>()->simList)
	//	{
	//
	//	}
	//	for (auto house : landmark.second[House])
	//	{
	//
	//	}
	//	for (auto light : landmark.second[Lighting])
	//	{
	//	}
	//}
}

void GuestGameWorld::gameTimeUpdate()
{
	if (GuestNetwork::network->isConnect)
		gameDeltaTime = Time::deltaTime * timeSpeed;
	else
		gameDeltaTime = 0;


	gameTime += gameDeltaTime;

	if (gameTime >= timeOfDay)
	{
		sun->transform->forward = { 1, 0, 0 };
		gameTime -= timeOfDay;
	}

	calculateSunInfo();
}

void GuestGameWorld::buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	buildingList[landmark][(BuildingType)type].push_back(building);
}

void GuestGameWorld::deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	if (type == BuildingType::Landmark)
	{
		for (auto& list : GuestGameWorld::gameWorld->buildingList[building])
		{
			for (auto& object : list.second)
			{
				Scene::scene->PushDelete(object);
			}
		}
		buildingList.erase(building);
	}
	else
	{
		Scene::scene->PushDelete(building);
		buildingList[landmark][(BuildingType)type].erase(find(buildingList[landmark][(BuildingType)type].begin(), buildingList[landmark][(BuildingType)type].end(), building));
	}
}