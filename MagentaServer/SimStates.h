#pragma once
#include "main.h"

class IdleState : public State<Sim>
{
public:
	static IdleState* Instance();

	virtual void Enter(Sim* sim);
	virtual void Execute(Sim* sim);
	virtual void Exit(Sim* sim);

	//virtual bool OnMessage(Sim* sim, const Telegram& telegram);
	virtual bool OnMessage(Sim* sim, const SIM_Message& telegram);
};

class MoveState : public State<Sim>
{
public:
	static MoveState* Instance();

	virtual void Enter(Sim* sim);
	virtual void Execute(Sim* sim);
	virtual void Exit(Sim* sim);

	//virtual bool OnMessage(Sim* sim, const Telegram& telegram);
	virtual bool OnMessage(Sim* sim, const SIM_Message& telegram);
};

class SleepState : public State<Sim>
{
public:
	static SleepState* Instance();

	virtual void Enter(Sim* sim);
	virtual void Execute(Sim* sim);
	virtual void Exit(Sim* sim);

	//virtual bool OnMessage(Sim* sim, const Telegram& telegram);
	virtual bool OnMessage(Sim* sim, const SIM_Message& telegram);
};

class BuildState : public State<Sim>
{
public:
	static BuildState* Instance();

	virtual void Enter(Sim* sim);
	virtual void Execute(Sim* sim);
	virtual void Exit(Sim* sim);

	//virtual bool OnMessage(Sim* sim, const Telegram& telegram);
	virtual bool OnMessage(Sim* sim, const SIM_Message& telegram);
};