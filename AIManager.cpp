#include "pch.h"
#include "AIManager.h"


void AIManager::Start(/*초기화 코드를 작성하세요.*/)
{
	aiManager = this;
}

void AIManager::Update(/*업데이트 코드를 작성하세요.*/)
{
}

void AIManager::aiUpdate()
{
	float time = GameWorld::gameWorld->gameTime;


	for (auto sim : GameWorld::gameWorld->simList)
	{
		sim.second->GetComponent<Sim>()->stateMachine.Update();
	}

	//if (time > 30.f)
	//{
	//	for (auto sim : GameWorld::gameWorld->simList)
	//	{
	//		Messenger->CreateMessage(0, sim.first, sim.first, Msg_Sleep);
	//	}
	//}

	for (auto landmark : GameWorld::gameWorld->buildingList)
	{
		Village* village = landmark.first->GetComponent<Village>();

		if (village->autoDevelopment && !village->simList.empty())
		{
			if (village->delayTime <= 0.f)
			{
				BuildMessageInfo* info = new BuildMessageInfo;
				info->pos = Vector2(landmark.first->transform->position.x + (rand() % 30) - 15, landmark.first->transform->position.z + (rand() % 30) - 15);
				info->buildingType = rand() % 2 + 3;
				info->buildingIndex = rand() % BuildingBuilder::buildingBuilder->getBuildingCount(info->buildingType);

				village->delayTime = rand() % 10 + 30;
				Messenger->CreateMessage(0, rand() % village->simList.size(), rand() % village->simList.size(), Msg_Build, info);
			}

			village->delayTime -= Time::deltaTime;
		}
	}


	Messenger->Timer();
}