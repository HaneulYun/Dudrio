#pragma once

class SimManager : public Singleton<SimManager>
{
private:
	int simIndex = 0;

public:
	map<int, Sim*> sims;

public:
	SimManager()  {}
	~SimManager() {}

	int AddSim(Sim* sim)
	{
		sim->id = simIndex;
		sims[simIndex++] = sim;

		sim->stateMachine.PushState(IdleState::Instance());
		sim->stateMachine.GetCurrentState()->Enter(sim);

		return simIndex;
	}
};