#pragma once
#include "Message.h"

template <typename entity>
class State
{
public:
	virtual void Enter(entity*) = 0;
	virtual void Execute(entity*) = 0;
	virtual void Exit(entity*) = 0;

	virtual bool OnMessage(entity*, const SIM_Message&) = 0;
};