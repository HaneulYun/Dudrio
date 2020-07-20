#pragma once

template <typename entity>
class State
{
public:
	virtual void Enter(entity*) = 0;
	virtual void Execute(entity*) = 0;
	virtual void Exit(entity*) = 0;

	virtual bool OnMessage(entity*, const Telegram&) = 0;

};