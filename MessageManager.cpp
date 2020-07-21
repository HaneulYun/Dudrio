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
		int receiverIndex = telegram.receiver;

		if (telegram.msg == Msg_Build)
		{
			// 메시지를 보내는 순간에 유효한 심을 검색해서 메시지 전달
			BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);
			receiverIndex = info->simList->at(rand() % info->simList->size())->GetComponent<Sim>()->id;
		}

		Sim* receiver = AIManager::Instance->sims[receiverIndex];
		if (receiver == nullptr)
			return;

		Send(receiver, telegram);

		if (telegram.extraInfo != nullptr)
			delete telegram.extraInfo;

		msgQueue.erase(msgQueue.begin());
	}
}