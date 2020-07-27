#include "AIManager.h"

AIManager::AIManager()
{
	aiManager = this;
}

void AIManager::AIUpdate()
{
	float time = contents.ingame_time;

	if (time > 30.f)
	{
		for (auto sim : contents.simList)
		{
			Messenger->CreateMessage(0, sim.first, sim.first, Msg_Sleep);
		}
		contents.ingame_time -= 30.f;
	}

	for (auto landmark : GameWorld::gameWorld->buildingList)
	{
		Village* village = landmark.first->GetComponent<Village>();

		if (village->autoDevelopment && !village->simList.empty())
		{
			if (village->delayTime <= 0.f)
			{
				BuildMessageInfo* info = new BuildMessageInfo;
				info->pos = Vector2D(landmark.first->transform->position.x + (rand() % 30) - 15, landmark.first->transform->position.z + (rand() % 30) - 15);
				info->buildingType = rand() % 2 + 3;
				info->buildingIndex = rand() % contents.collider_info[info->buildingType].size();

				village->delayTime = rand() % 10 + 30;
				Messenger->CreateMessage(0, rand() % village->simList.size(), rand() % village->simList.size(), Msg_Build, info);
			}

			village->delayTime -= Time::deltaTime;
		}
	}

	Messenger->Timer();
}