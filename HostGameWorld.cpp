#include "pch.h"
#include "HostGameWorld.h"

void HostGameWorld::Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
{
	GameWorld::Start();
	gameWorld = this;
}

void HostGameWorld::Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
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

void HostGameWorld::aiUpdate()
{
	if (!simList.empty())
		AIManager::aiManager->aiUpdate();
}

void HostGameWorld::gameTimeUpdate()
{
	if (gameTime >= timeOfDay)
	{
		sun->transform->forward = { 1, 0, 0 };
		gameTime -= timeOfDay;
		++day;
	}

	if(!HostNetwork::network->isConnect)
		gameDeltaTime = MathHelper::Clamp(Time::deltaTime * timeSpeed, 0.0f, 0.025f * timeSpeed);
	else
		gameDeltaTime = Time::deltaTime * timeSpeed;

	gameTime += gameDeltaTime;

	calculateSunInfo();
}

void HostGameWorld::buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	buildingList[landmark][(BuildingType)type].push_back(building);

	if (type == BuildingType::House && !HostNetwork::network->isConnect)
		addSim(landmark, building);
}

void HostGameWorld::deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Prop)
	{
		if (building->GetComponent<Light>())
			type = BuildingType::Lighting;
	}
	if (type == BuildingType::Landmark)
	{
		for (auto& list : HostGameWorld::gameWorld->buildingList[building])
		{
			for (auto& object : list.second)
			{
				Vector3 building_forward = object->transform->forward;
				building_forward.y = 0;
				building_forward.Normalize();
				Vector3 forward = { 0,0,1 };
				float angle = Vector3::DotProduct(forward, building_forward);
				Vector3 dir = Vector3::CrossProduct(forward, building_forward);
				angle = XMConvertToDegrees(acos(angle));
				angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;

				int range = 0;
				if (object->GetComponent<Village>() != nullptr)
					range = object->GetComponent<Village>()->radiusOfLand;
				GameLoader::gameLoader->deleteInFile(type, index, object->transform->position.x, object->transform->position.z, angle, range);
				Scene::scene->PushDelete(object);
				BuildingBuilder::buildingBuilder->updateTerrainNodeData(object, false);

				if (list.first == BuildingType::House && !HostNetwork::network->isConnect)
					eraseSim(landmark, object);
			}
		}
		buildingList.erase(building);
	}
	else
	{
		Vector3 building_forward = building->transform->forward;
		building_forward.y = 0;
		building_forward.Normalize();
		Vector3 forward = { 0,0,1 };
		float angle = Vector3::DotProduct(forward, building_forward);
		Vector3 dir = Vector3::CrossProduct(forward, building_forward);
		angle = XMConvertToDegrees(acos(angle));
		angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;

		GameLoader::gameLoader->deleteInFile(type, index, building->transform->position.x, building->transform->position.z, angle, 0);
		Scene::scene->PushDelete(building);
		BuildingBuilder::buildingBuilder->updateTerrainNodeData(building, false);
		buildingList[landmark][(BuildingType)type].erase(find(buildingList[landmark][(BuildingType)type].begin(), buildingList[landmark][(BuildingType)type].end(), building));
	}
	if (type == BuildingType::House && !HostNetwork::network->isConnect)
		eraseSim(landmark, building);
}

int HostGameWorld::addSim(GameObject* landmark, GameObject* house)
{
	GameObject* sim = Scene::scene->Duplicate(simPrefab);
	sim->transform->position = house->transform->position;

	auto simComponent = sim->GetComponent<Sim>();
	simComponent->animator = sim->GetComponent<Animator>();
	simComponent->home = house;
	simComponent->id = simIndex;
	simList[simIndex++] = sim;

	simComponent->stateMachine.setOwner(simComponent);
	simComponent->stateMachine.PushState(IdleState::Instance());
	simComponent->stateMachine.GetCurrentState()->Enter(simComponent);

	landmark->GetComponent<Village>()->simList[house] = sim;

	return simIndex;
}

int HostGameWorld::eraseSim(GameObject* landmark, GameObject* house)
{
	GameObject* sim = landmark->GetComponent<Village>()->simList[house];

	int id = sim->GetComponent<Sim>()->id;

	landmark->GetComponent<Village>()->simList.erase(house);
	simList.erase(id);

	Scene::scene->PushDelete(sim);
	return id;
}