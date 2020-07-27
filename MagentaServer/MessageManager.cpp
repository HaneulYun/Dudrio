#include "MessageManager.h"


void MessageManager::Send(Sim* receiver,
	const Telegram& telegram)
{
	receiver->HandleMessage(telegram);
}

void MessageManager::CreateMessage(double delay, int senderIndex, int receiverIndex, int msg, void* ExtraInfo)
{
	Telegram telegram(0, senderIndex, receiverIndex, msg, ExtraInfo);
           
	float currentTime = GetTickCount64();
	telegram.dispatchTime = currentTime + delay;

	msgQueue.insert(telegram);
}

void MessageManager::Timer()
{
	float currentTime = GetTickCount64();

	while (!msgQueue.empty() && (msgQueue.begin()->dispatchTime < currentTime))
	{
		const Telegram& telegram = *msgQueue.begin();

		//Sim* receiver = AIManager::Instance->simList[telegram.receiver];
		Sim* receiver = GameWorld::gameWorld->simList[telegram.receiver]->GetComponent<Sim>();
		if (receiver == nullptr)
			return;

		Send(receiver, telegram);

		if (telegram.extraInfo != nullptr)
			delete telegram.extraInfo;

		msgQueue.erase(msgQueue.begin());
	}
}