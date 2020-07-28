#include "pch.h"
#include "GuestGameWorld.h"

void GuestGameWorld::Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
{
	gameWorld = this;
}

void GuestGameWorld::Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
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
	gameTime += Time::deltaTime;
	gameDeltaTime = Time::deltaTime;
}

void GuestGameWorld::buildInGameWorld(GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	buildingList[(BuildingType)type].push_back(building);
}

void GuestGameWorld::deleteInGameWorld(GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	buildingList[(BuildingType)type].erase(find(buildingList[(BuildingType)type].begin(), buildingList[(BuildingType)type].end(), building));
}