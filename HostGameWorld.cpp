#include "pch.h"
#include "HostGameWorld.h"

void HostGameWorld::Start(/*초기화 코드를 작성하세요.*/)
{
	GameWorld::Start();
	gameWorld = this;

	if (!HostInformConnector::connector->load)
		BuildingBuilder::buildingBuilder->initNature();
}

void HostGameWorld::Update(/*업데이트 코드를 작성하세요.*/)
{
	if (Input::GetKeyDown(KeyCode::Return))
		changeMode(ChatMode);

	if (Input::GetKeyDown(KeyCode::Z))
		changeMode(MenuMode);

	if (gameState != MenuMode || HostNetwork::network->isConnect)
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
	if (!HostNetwork::network->isConnect)
		gameUI->gameUIs[GameUI::GameUICategory::SimCountUI]->GetComponent<Text>()->text = to_wstring(simList.size());
	else
		gameUI->gameUIs[GameUI::GameUICategory::SimCountUI]->GetComponent<Text>()->text = to_wstring(HostNetwork::network->sims.size());
	gameUI->gameUIs[GameUI::GameUICategory::CoinCountUI]->GetComponent<Text>()->text = to_wstring(gameMoney);

	if (gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->active)
	{
		if (BuildingBuilder::buildingBuilder->curLandmark == nullptr || BuildingBuilder::buildingBuilder->curPrefabType == 0)
		{
			gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->GetComponentInChildren<Text>()->text = L". . .m\n\n. . .명";
		}
		else if(BuildingBuilder::buildingBuilder->curLandmark != nullptr)
		{
			Village* village = BuildingBuilder::buildingBuilder->curLandmark->GetComponent<Village>();
			if(!HostNetwork::network->isConnect)
				gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->GetComponentInChildren<Text>()->text = to_wstring(village->radiusOfLand) + L"m\n\n" + to_wstring(village->simList.size()) + L"명";
			else if(village != nullptr)
				gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->GetComponentInChildren<Text>()->text = to_wstring(village->radiusOfLand) + L"m\n\n" + to_wstring(village->serverSimList.size()) + L"명";
		}
	}
	
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
	if (landmark == nullptr)
		type = Nature;
	buildingList[landmark][(BuildingType)type].push_back(building);

	if (type == BuildingType::House && !HostNetwork::network->isConnect)
		addSim(landmark, building);
}

void HostGameWorld::deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
	if (type == BuildingType::Landmark)
	{
		for (auto& list : HostGameWorld::gameWorld->buildingList[building])
		{
			int obj_type = list.first;
			for (auto& object : list.second)
			{
				Vector3 building_forward = object->transform->forward;
				building_forward.y = 0;
				building_forward.Normalize();
				Vector3 forward = { 0,0,1 };
				double angle = Vector3::DotProduct(forward, building_forward);
				Vector3 dir = Vector3::CrossProduct(forward, building_forward);
				angle = XMConvertToDegrees(acos(angle));
				angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;

				int range = 0;
				if (object->GetComponent<Village>() != nullptr) {
					range = object->GetComponent<Village>()->radiusOfLand;
					object->GetComponent<Village>()->serverSimList.clear();
				}
				GameLoader::gameLoader->deleteInFile(obj_type, object->GetComponent<Building>()->index, object->transform->position.x, object->transform->position.z, angle, range);
				GameLoader::gameLoader->SaveTime(gameTime, day);
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
		GameLoader::gameLoader->SaveTime(gameTime, day);
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
			auto chatField = gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->GetComponent<InputField>();
			if (HostNetwork::network->isConnect && HostNetwork::network->mainConnect) {
				if (chatField->text.size() > MAX_STR_LEN - 1) {
					int oversize = chatField->text.size() - (MAX_STR_LEN - 1);
					for (int i = 0; i < oversize; ++i)
						chatField->text.pop_back();
				}
				if (!chatField->text.empty())
					HostNetwork::network->send_chat_packet(_wcsdup(chatField->text.c_str()));
			}
			gameState = CameraMode;
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->SetActive(false);
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->GetComponent<InputField>()->isFocused = false;
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->GetComponent<InputField>()->clear();
		}
		else if(gameState != MenuMode)
		{
			gameState = ChatMode;
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->SetActive(true);
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->GetComponent<InputField>()->isFocused = true;
			gameUI->gameUIs[GameUI::GameUICategory::ChatUI]->GetComponent<InputField>()->clear();
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