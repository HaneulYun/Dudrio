#include "pch.h"
#include "SimStates.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////                                                       
// Idle : 가만히 있는 중
/////////////////////////////////////////////////////////////////////////////////////////////////////
IdleState* IdleState::Instance()
{
	static IdleState instance;

	return &instance;
}

void IdleState::Enter(Sim* sim)
{

	if (sim->stateMachine.HaveNextState())
		return;

	double delay = rand() % 10;
	Messenger->CreateMessage(delay, sim->id, sim->id, Msg_Move);
};

void IdleState::Execute(Sim* sim)
{
	sim->animator->SetFloat("Walking", 0);

};

void IdleState::Exit(Sim* sim)
{

};

bool IdleState::OnMessage(Sim* sim, const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case Msg_Move:
		Vector2 targetPos;
		do
		{
			targetPos = Vector2(sim->gameObject->transform->position.x - (rand() % 30) + 15, sim->gameObject->transform->position.z - (rand() % 30) + 15);
		} while (BuildingBuilder::buildingBuilder->terrainNodeData->extraData[(int)targetPos.x + (int)targetPos.y * 1024].collision);

		sim->targetPos.push_back(targetPos);
		sim->stateMachine.ChangeState(MoveState::Instance());
		return true;
	case Msg_Sleep:
		sim->stateMachine.ClearStack();
		sim->targetPos.clear();
		sim->path.clear();

		sim->targetPos.push_back(Vector2(sim->home->transform->position.x, sim->home->transform->position.z));
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	case Msg_Build:
	{
		sim->targetPos.clear();
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);

		sim->targetPos.push_back(info->pos);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());

		sim->stateMachine.ChangeState();
	}
		return true;
	}
	return false;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////
// Move : 움직이는 중
/////////////////////////////////////////////////////////////////////////////////////////////////////
MoveState* MoveState::Instance()
{
	static MoveState instance;

	return &instance;
}

void MoveState::Enter(Sim* sim)
{
	if (sim->stateMachine.IsNextState(BuildState::Instance()))
		PathFinder::Instance()->FindPath(sim->targetPos.front(), Vector2(sim->gameObject->transform->position.x, sim->gameObject->transform->position.z), sim->path,
			BuildingBuilder::buildingBuilder->getBoundingBox(sim->buildInfo.buildingType, sim->buildInfo.buildingIndex) + 1);
	else
		PathFinder::Instance()->FindPath(sim->targetPos.front(), Vector2(sim->gameObject->transform->position.x, sim->gameObject->transform->position.z), sim->path);
};

void MoveState::Execute(Sim* sim)
{
	sim->animator->SetFloat("Walking", 2);

	if (sim->path.empty())
	{
		sim->targetPos.pop_front();

		if (sim->stateMachine.HaveNextState())
			sim->stateMachine.ChangeState();
		else
			sim->stateMachine.ChangeState(IdleState::Instance());
		return;
	}


	Vector2 distanceV{ sim->gameObject->transform->position.x - sim->path.front().x, sim->gameObject->transform->position.z - sim->path.front().y };
	float distance = sqrt(pow(distanceV.x, 2) + pow(distanceV.y, 2));
	
	if (distance < 0.1f)
	{
		sim->path.pop_front();
	}
	else
		PathFinder::Instance()->MoveToDestination(sim->path.front(), sim->gameObject->transform, sim->speed);
};

void MoveState::Exit(Sim* sim)
{
};

bool MoveState::OnMessage(Sim* sim, const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case Msg_Move:
		return true;
	case Msg_Sleep:
		sim->stateMachine.ClearStack();
		sim->targetPos.clear();
		sim->path.clear();

		Vector2 dest;
		dest.x = sim->home->transform->position.x;
		dest.y = sim->home->transform->position.z;
		sim->targetPos.push_back(dest);

		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	case Msg_Build:
	{
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);

		sim->targetPos.push_back(info->pos);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());
	}
		return true;
	}
	return false;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Sleep : 자는 중
/////////////////////////////////////////////////////////////////////////////////////////////////////
SleepState* SleepState::Instance()
{
	static SleepState instance;

	return &instance;
}

void SleepState::Enter(Sim* sim)
{
	Messenger->CreateMessage(15, sim->id, sim->id, Msg_WakeUp);
};

void SleepState::Execute(Sim* sim)
{

};

void SleepState::Exit(Sim* sim)
{
};

bool SleepState::OnMessage(Sim* sim, const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case Msg_Move:
		return true;
	case Msg_Build:
	{
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);

		sim->targetPos.push_back(info->pos);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());
	}
		return true;
	case Msg_WakeUp:
		if (sim->stateMachine.HaveNextState())
			sim->stateMachine.ChangeState();
		else
		{
			sim->targetPos.push_back(Vector2(sim->gameObject->transform->position.x - (rand() % 10 - 5), sim->gameObject->transform->position.z - (rand() % 10 - 5)));
			sim->stateMachine.ChangeState(IdleState::Instance());
		}
		return true;
	}
	return false;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Sleep : 건설 중
/////////////////////////////////////////////////////////////////////////////////////////////////////
BuildState* BuildState::Instance()
{
	static BuildState instance;

	return &instance;
}

void BuildState::Enter(Sim* sim)
{
};

void BuildState::Execute(Sim* sim)
{
	// 건물 건설
	BuildingBuilder::buildingBuilder->build(sim->buildInfo.pos, 0, sim->buildInfo.buildingType, sim->buildInfo.buildingIndex);

	// 다음 상태로 전이
	if (sim->stateMachine.HaveNextState())
		sim->stateMachine.ChangeState();
	else
		sim->stateMachine.ChangeState(IdleState::Instance());
};

void BuildState::Exit(Sim* sim)
{
};

bool BuildState::OnMessage(Sim* sim, const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case Msg_Sleep:
		Vector2 dest;
		dest.x = sim->home->transform->position.x;
		dest.y = sim->home->transform->position.z;
		sim->targetPos.push_back(dest);

		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	return false;
};