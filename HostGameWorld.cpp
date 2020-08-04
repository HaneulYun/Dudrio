#include "pch.h"
#include "HostGameWorld.h"

void HostGameWorld::Start(/*초기화 코드를 작성하세요.*/)
{
	GameWorld::Start();
	gameWorld = this;
}

void HostGameWorld::Update(/*업데이트 코드를 작성하세요.*/)
{
	if (Input::GetKeyDown(KeyCode::Return))
		changeMode(ChatMode);

	if (Input::GetKeyDown(KeyCode::Z))
		changeMode(MenuMode);

	if (gameState != MenuMode && !HostNetwork::network->isConnect)
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

	uiUpdate();

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

void HostGameWorld::uiUpdate()
{
	gameUI->gameUIs[GameUI::GameUICategory::DayAndTimeUI]->GetComponent<Text>()->text = convertTimeToText() + L" ,   DAY " + to_wstring(day) + L"\t";
	gameUI->gameUIs[GameUI::GameUICategory::SimCountUI]->GetComponent<Text>()->text = to_wstring(simList.size());
	gameUI->gameUIs[GameUI::GameUICategory::CoinCountUI]->GetComponent<Text>()->text = to_wstring(gameMoney);
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

void HostGameWorld::changeMode(GameState state)
{
	switch (state)
	{

	case CameraMode:
	{

	}
	break;
	case ChatMode:
	{
		if (gameState == state)
		{
			gameState = CameraMode;
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->SetActive(false);
		}
		else
		{
			gameState = ChatMode;
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->SetActive(true);
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->GetComponent<InputField>()->isFocused = true;
			memset(Input::buffer, 0, 8);
		}
	}
	break;
	case MenuMode:
	{
		if (gameState == state)
		{
			gameState = CameraMode;
			gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->SetActive(false);
		}
		else
		{
			gameState = MenuMode;
			gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->SetActive(true);
		}
	}
	break;

	}

}