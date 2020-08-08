#include "pch.h"
#include "GuestGameWorld.h"

void GuestGameWorld::Start(/*초기화 코드를 작성하세요.*/)
{
	GameWorld::Start();
	gameWorld = this;
}

void GuestGameWorld::Update(/*업데이트 코드를 작성하세요.*/)
{
	if (Input::GetKeyDown(KeyCode::Return))
		changeMode(ChatMode);

	if (Input::GetKeyDown(KeyCode::Z))
		changeMode(MenuMode);

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
	
	uiUpdate();
}

void GuestGameWorld::uiUpdate()
{
	guestUI->guestUIs[GuestUI::GuestUICategory::DayAndTimeUI]->GetComponent<Text>()->text = L"\t" + convertTimeToText();
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
	if (landmark == nullptr)
		type = Nature;
	buildingList[landmark][(BuildingType)type].push_back(building);
}

void GuestGameWorld::deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
{
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

void GuestGameWorld::changeMode(GameState state)
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
			guestUI->guestUIs[GuestUI::GuestUICategory::ChatUI]->SetActive(false);
			guestUI->guestUIs[GuestUI::GuestUICategory::ChatUI]->GetComponent<InputField>()->isFocused = false;
			guestUI->guestUIs[GuestUI::GuestUICategory::ChatUI]->GetComponent<InputField>()->clear();
		}
		else if (gameState != MenuMode)
		{
			gameState = ChatMode;
			guestUI->guestUIs[GuestUI::GuestUICategory::ChatUI]->SetActive(true);
			guestUI->guestUIs[GuestUI::GuestUICategory::ChatUI]->GetComponent<InputField>()->isFocused = true;
			guestUI->guestUIs[GuestUI::GuestUICategory::ChatUI]->GetComponent<InputField>()->clear();
			memset(Input::buffer, 0, 8);
		}
	}
	break;
	case MenuMode:
	{
		if (gameState == state)
		{
			gameState = CameraMode;
			guestUI->guestUIs[GuestUI::GuestUICategory::MenuUI]->SetActive(false);
		}
		else
		{
			gameState = MenuMode;
			guestUI->guestUIs[GuestUI::GuestUICategory::MenuUI]->SetActive(true);
		}
	}
	break;

	}

}