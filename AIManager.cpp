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


	//for (auto sim : GameWorld::gameWorld->simList)
	//{
	//	sim.second->GetComponent<Sim>()->stateMachine.Update();
	//}

	if (time > 24.0f * 37.5f)
	{
		for (auto sim : GameWorld::gameWorld->simList)
		{
			Messenger->CreateMessage(0, sim.first, sim.first, Msg_Sleep);
		}
	}

	for (auto landmark : GameWorld::gameWorld->buildingList)
	{
		Village* village = landmark.first->GetComponent<Village>();

		if (village->autoDevelopment && !village->simList.empty())
		{
			if (village->delayTime <= 0.f)
			{
				BuildMessageInfo* info = new BuildMessageInfo;
				info->pos = Vector2(landmark.first->transform->position.x + (rand() % village->radiusOfLand) - village->radiusOfLand / 2, landmark.first->transform->position.z + (rand() % village->radiusOfLand) - village->radiusOfLand / 2);
				info->buildingType = rand() % 2 + 3;
				info->buildingIndex = rand() % BuildingBuilder::buildingBuilder->getBuildingCount(info->buildingType);

				village->delayTime = rand() % 120 + 60;
				int index = rand() % village->simList.size();
				int id = village->simList[landmark.second[GameWorld::gameWorld->House][index]]->GetComponent<Sim>()->id;
				Messenger->CreateMessage(5, id, id, Msg_Build, info);
			}
			village->delayTime -= GameWorld::gameWorld->gameDeltaTime;
		}
	}


	Messenger->Timer();
}