#pragma once

template <typename entity>
class StateMachine
{
private:
	entity* owner;
	deque<State<entity>*> stateStack;

public:
	StateMachine(entity* owner) : owner(owner) {}

	State<entity>* GetCurrentState() const
	{
		return stateStack.front();
	}

	void Update()const
	{
		if (!stateStack.empty()) stateStack.front()->Execute(owner);
	}

	bool HandleMessage(const Telegram& msg)const
	{
		return (!stateStack.empty() && stateStack.front()->OnMessage(owner, msg));
		return false;
	}

	void ClearStack()
	{
		stateStack.clear();
	}

	bool HaveNextState()
	{
		return stateStack.size() > 1;
	}

	void PushState(State<entity>* newState)
	{
		stateStack.push_back(newState);
	}

	void ChangeState()
	{
		stateStack.front()->Exit(owner);
		stateStack.pop_front();
		stateStack.front()->Enter(owner);
	}

	void ChangeState(State<entity>* newState)
	{
		stateStack.front()->Exit(owner);
		stateStack.pop_front();
		stateStack.push_back(newState);
		stateStack.front()->Enter(owner);
	}
};