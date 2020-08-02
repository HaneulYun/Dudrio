#include "pch.h"
#include "MessageManager.h"


void MessageManager::Send(Sim* receiver,
	const Telegram& telegram)
{
	receiver->HandleMessage(telegram);
}

void MessageManager::CreateMessage(double delay, int senderIndex, int receiverIndex, int msg, void* ExtraInfo)
{
	Telegram telegram(0, senderIndex, receiverIndex, msg, ExtraInfo);
           
	//float currentTime = Time::currentTime;
	float currentTime = GameWorld::gameWorld->day * 37.5f * 24 + GameWorld::gameWorld->gameTime;
	telegram.dispatchTime = currentTime + delay;

	msgQueue.insert(telegram);
}

void MessageManager::Timer()
{
	//float currentTime = Time::currentTime;
	float currentTime = GameWorld::gameWorld->day * 37.5f * 24 + GameWorld::gameWorld->gameTime;

	while (!msgQueue.empty() && (msgQueue.begin()->dispatchTime < currentTime))
	{
		const Telegram& telegram = *msgQueue.begin();

		if (GameWorld::gameWorld->simList.count(telegram.receiver) != 0)
		{
			Sim* receiver = GameWorld::gameWorld->simList[telegram.receiver]->GetComponent<Sim>();
			if (receiver == nullptr)
				return;

			Send(receiver, telegram);
		}

		if (telegram.extraInfo != nullptr)
			delete telegram.extraInfo;

		msgQueue.erase(msgQueue.begin());
	}
}