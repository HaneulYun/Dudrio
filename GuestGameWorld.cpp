#include "pch.h"
#include "GuestGameWorld.h"

void GuestGameWorld::Start(/*초기화 코드를 작성하세요.*/)
{
	gameWorld = this;
	sun->transform->Rotate({ 1, 0, 0 }, 90);
	sun->transform->Rotate({ 0, 1, 0 }, -90);
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

void GuestGameWorld::calculateSunInfo()
{
	Light* light = sun->GetComponent<Light>();
	light->Strength = { 0.9, 0.9, 0.9 };

	float sunRotAngle = 0.0f;
	if (gameTime < 6 * 37.5f) {
		sunRotAngle += 0.8 * gameTime;
	}
	else if (gameTime < 10 * 37.5f) {
		sunRotAngle += 0.8 * 6 * 37.5f;
		sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
		//light->Strength += (gameTime - (6 * 37.5f)) * 0.008f;
		//if (light->Strength.x > 0.9f)
		//	light->Strength = { 0.9, 0.9, 0.9 };
	}
	else if (gameTime < 21 * 37.5f) {
		sunRotAngle += 0.8 * 6 * 37.5f;
		sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
		//light->Strength = { 0.9, 0.9, 0.9 };
	}
	else {
		sunRotAngle += 0.8 * 6 * 37.5f;
		sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
		//light->Strength = { 0.9, 0.9, 0.9 };
		//light->Strength -= (gameTime - (21 * 37.5f)) * 0.008f;
		//if (light->Strength.x < 0.0f)
		//	light->Strength = { 0,0,0 };
	}

	sun->transform->forward = Vector3(1, 0, 0).TransformNormal(Matrix4x4::RotationZ(XMConvertToRadians(sunRotAngle)));
}

std::wstring GuestGameWorld::convertTimeToText()
{
	int hour;
	int minute;

	hour = std::floor(gameTime / 37.5f);
	minute = std::floor((gameTime - (hour * 37.5f)) / 0.625f);

	std::wstring text = to_wstring(hour) + L":" + to_wstring(minute);
	return text;
}