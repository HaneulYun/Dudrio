#include "pch.h"
#include "GameWorld.h"

void GameWorld::Start(/*초기화 코드를 작성하세요.*/)
{
	gameWorld = this;
	sun->transform->Rotate({ 1, 0, 0 }, 90);
	sun->transform->Rotate({ 0, 1, 0 }, -90);
}

void GameWorld::Update(/*업데이트 코드를 작성하세요.*/)
{
	gameTimeUpdate();

	if (!HostNetwork::network->isConnect){
		aiUpdate();

		if (Input::GetKeyDown(KeyCode::Alpha1))
			timeSpeed = X1;
		else if (Input::GetKeyDown(KeyCode::Alpha2))
			timeSpeed = X2;
		else if (Input::GetKeyDown(KeyCode::Alpha4))
			timeSpeed = X4;
		else if (Input::GetKeyDown(KeyCode::Alpha8))
			timeSpeed = X8;
	}


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

void GameWorld::aiUpdate()
{
	if (!simList.empty())
		AIManager::aiManager->aiUpdate();
}

void GameWorld::gameTimeUpdate()
{
	if(!HostNetwork::network->isConnect)
		gameDeltaTime = MathHelper::Clamp(Time::deltaTime * timeSpeed, 0.0f, 0.025f * timeSpeed);
	else
		gameDeltaTime = Time::deltaTime * timeSpeed;

	gameTime += gameDeltaTime;
	
	if (gameTime >= timeOfDay)
	{
		sun->transform->forward = { 1, 0, 0 };
		gameTime -= timeOfDay;
	}

	calculateSunInfo();
}

void GameWorld::buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	buildingList[landmark][(BuildingType)type].push_back(building);

	if (type == BuildingType::House &&!HostNetwork::network->isConnect)
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

	auto simComponent = sim->GetComponent<Sim>();
	simComponent->animator = sim->GetComponent<Animator>();
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

void GameWorld::calculateSunInfo()
{
	Light* light = sun->GetComponent<Light>();
	light->Strength = { 0,0,0 };

	float sunRotAngle = 0.0f;
	if (gameTime < 6 * 37.5f) {
		sunRotAngle += 0.8 * gameTime;
	}
	else if (gameTime < 10 * 37.5f) {
		sunRotAngle += 0.8 * 6 * 37.5f;
		sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
		light->Strength += (gameTime - (6 * 37.5f)) * 0.008f;
		if (light->Strength.x > 0.9f)
			light->Strength = { 0.9, 0.9, 0.9 };
	}
	else if (gameTime < 21 * 37.5f) {
		sunRotAngle += 0.8 * 6 * 37.5f;
		sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
		light->Strength = { 0.9, 0.9, 0.9 };
	}
	else {
		sunRotAngle += 0.8 * 6 * 37.5f;
		sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
		light->Strength = { 0.9, 0.9, 0.9 };
		light->Strength -= (gameTime - (21 * 37.5f)) * 0.008f;
		if (light->Strength.x < 0.0f)
			light->Strength = { 0,0,0 };
	}

	sun->transform->forward = Vector3(1, 0, 0).TransformNormal(Matrix4x4::RotationZ(XMConvertToRadians(sunRotAngle)));
}

std::wstring GameWorld::convertTimeToText()
{
	int hour;
	int minute;

	hour = std::floor(gameTime / 37.5f);
	minute = std::floor((gameTime - (hour * 37.5f)) / 0.625f);

	std::wstring text = to_wstring(hour) + L":" + to_wstring(minute);
	return text;
}