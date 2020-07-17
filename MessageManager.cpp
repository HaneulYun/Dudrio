#include "pch.h"
#include "MessageManager.h"


void MessageManager::Send(Sim* receiver,
	const Telegram& telegram)
{
	if (!receiver->HandleMessage(telegram))
		Debug::Log("\n메시지 전달 안됨");
}

void MessageManager::CreateMessage(double delay, int senderIndex, int receiverIndex, int msg, void* ExtraInfo)
{
	Sim* sender = SimManager::Instance()->sims[senderIndex];
	Sim* receiver = SimManager::Instance()->sims[receiverIndex];

	Telegram telegram(0, senderIndex, receiverIndex, msg, ExtraInfo);
           
	float currentTime = Time::currentTime;
	telegram.dispatchTime = currentTime + delay;

	msgQueue.insert(telegram);

	string str = "\nDelayed telegram from " + to_string(senderIndex) + " recorded at time " + to_string(Time::currentTime) + " for " + to_string(receiverIndex) + ". msg is " + to_string(msg);
	Debug::Log(str.c_str());
}

void MessageManager::Timer()
{
	float currentTime = Time::currentTime;

	while (!msgQueue.empty() && (msgQueue.begin()->dispatchTime < currentTime))
	{
		const Telegram& telegram = *msgQueue.begin();

		Sim* receiver = SimManager::Instance()->sims[telegram.receiver];

		string str = "\nQueued telegram ready for dispatch: Sent to " + to_string(telegram.receiver) + ". msg is " + to_string(telegram.msg);
		Debug::Log(str.c_str());

		Send(receiver, telegram);

		msgQueue.erase(msgQueue.begin());
	}
}