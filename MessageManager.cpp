#include "pch.h"
#include "MessageManager.h"


void MessageManager::Send(Sim* receiver,
	const Telegram& telegram)
{
	receiver->HandleMessage(telegram);
}

void MessageManager::CreateMessage(double delay, int senderIndex, int receiverIndex, int msg, void* ExtraInfo)
{
	Sim* sender = AIManager::Instance->sims[senderIndex];
	Sim* receiver = AIManager::Instance->sims[receiverIndex];

	Telegram telegram(0, senderIndex, receiverIndex, msg, ExtraInfo);
           
	float currentTime = Time::currentTime;
	telegram.dispatchTime = currentTime + delay;

	msgQueue.insert(telegram);
}

void MessageManager::Timer()
{
	float currentTime = Time::currentTime;

	while (!msgQueue.empty() && (msgQueue.begin()->dispatchTime < currentTime))
	{
		const Telegram& telegram = *msgQueue.begin();

		Sim* receiver = AIManager::Instance->sims[telegram.receiver];

		Send(receiver, telegram);

		msgQueue.erase(msgQueue.begin());
	}
}