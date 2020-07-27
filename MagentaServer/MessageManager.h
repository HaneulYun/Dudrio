#pragma once
#include <set>
#include "main.h"

#define Messenger MessageManager::Instance()

class MessageManager : public Singleton<MessageManager>
{
private:
	std::set<Telegram> msgQueue;

public:
	void Send(Sim* receiver, const Telegram& msg);
	void CreateMessage(double delay, int senderIndex, int receiverIndex, int msg, void* ExtraInfo = NULL);
	void Timer();
};