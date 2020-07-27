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
	//sim->animator->SetFloat("Walking", 0);

};

void IdleState::Exit(Sim* sim)
{

};

bool IdleState::OnMessage(Sim* sim, const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case Msg_Move:
	{
		Vector2D targetPos;
		do
		{
			targetPos = Vector2D(sim->pos.x - (rand() % 16) + 8, sim->pos.z - (rand() % 16) + 8);
		} while (PathFinder::Instance()->terrainNodeData->extraData[(int)targetPos.x + (int)targetPos.z * 1024].collision);

		sim->targetPos.emplace_back(targetPos, 0);
		sim->stateMachine.ChangeState(MoveState::Instance());
		return true;
	}
	case Msg_Sleep:
	{
		sim->stateMachine.ClearStack();
		sim->targetPos.clear();
		sim->path.clear();

		// 집 주변까지 이동 + 집 안으로 이동
		TargetInfo targetInfo;
		targetInfo.pos = Vector2D(sim->home->m_info.m_xPos, sim->home->m_info.m_zPos);
		targetInfo.posOffset = sim->home->m_collider.getMaxBound() * 2.0f;

		sim->targetPos.push_back(targetInfo);
		sim->stateMachine.PushState(MoveState::Instance());

		sim->targetPos.emplace_back(Vector2D(sim->home->m_info.m_xPos, sim->home->m_info.m_zPos), 0);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	case Msg_Build:
	{
		sim->targetPos.clear();
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);

		sim->targetPos.emplace_back(info->pos, contents.collider_info[sim->buildInfo.buildingType][sim->buildInfo.buildingIndex].getMaxBound() + 1);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
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
	float targetPosOffset = sim->targetPos.front().posOffset;
	int checkCollisionOn = true;

	//if (sim->stateMachine.IsNextState(BuildState::Instance()))
	//	targetPosOffset = BuildingBuilder::buildingBuilder->getBoundingBox(sim->buildInfo.buildingType, sim->buildInfo.buildingIndex) + 1;
	
	if (sim->stateMachine.IsNextState(SleepState::Instance()))
		checkCollisionOn = false;


	if (PathFinder::Instance()->FindPath(sim->targetPos.front().pos, Vector2D(sim->pos.x, sim->pos.z), sim->path, checkCollisionOn, targetPosOffset) == false)
	{
		if (sim->stateMachine.HaveNextState() == false)
			sim->stateMachine.PushState(IdleState::Instance());
		sim->stateMachine.ChangeState();
	}
};

void MoveState::Execute(Sim* sim)
{
	if (sim->path.empty())
	{
		sim->targetPos.pop_front();

		sim->stateMachine.ChangeState();
		return;
	}

	Vector2D distanceV{ sim->pos.x - sim->path.front().x, sim->pos.z - sim->path.front().z };
	float distance = sqrt(pow(distanceV.x, 2) + pow(distanceV.z, 2));
	
	if (distance < 0.1f)
	{
		sim->path.pop_front();
	}
	else
		PathFinder::Instance()->MoveToDestination(sim->path.front(), sim, sim->speed);
};

void MoveState::Exit(Sim* sim)
{
	if (sim->stateMachine.HaveNextState() == false)
		sim->stateMachine.PushState(IdleState::Instance());
};

bool MoveState::OnMessage(Sim* sim, const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case Msg_Move:
		return true;
	case Msg_Sleep:
	{
		sim->stateMachine.ClearStack();
		sim->targetPos.clear();
		sim->path.clear();

		// 집 주변까지 이동 + 집 안으로 이동
		TargetInfo targetInfo;
		targetInfo.pos = Vector2D(sim->home->m_info.m_xPos, sim->home->m_info.m_zPos);
		targetInfo.posOffset = sim->home->m_collider.getMaxBound() * 2.0f;

		sim->targetPos.push_back(targetInfo);
		sim->stateMachine.PushState(MoveState::Instance());

		sim->targetPos.emplace_back(Vector2D(sim->home->m_info.m_xPos, sim->home->m_info.m_zPos), 0);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	case Msg_Build:
	{
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);

		sim->targetPos.emplace_back(info->pos, contents.collider_info[sim->buildInfo.buildingType][sim->buildInfo.buildingIndex].getMaxBound() + 1);
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
	Messenger->CreateMessage(5, sim->id, sim->id, Msg_WakeUp);
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

		sim->targetPos.emplace_back(info->pos, contents.collider_info[sim->buildInfo.buildingType][sim->buildInfo.buildingIndex].getMaxBound() + 1);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());
		return true;
	}
	case Msg_WakeUp:
	{
		if (sim->stateMachine.HaveNextState())
			sim->stateMachine.ChangeState();
		else
		{
			Vector2D targetPos;
			do
			{
				targetPos = Vector2D(sim->pos.x - (rand() % 16) + 8, sim->pos.z - (rand() % 16) + 8);
			} while (contents.terrainNodeData->extraData[(int)targetPos.x + (int)targetPos.z * 1024].collision);
			sim->targetPos.emplace_back(targetPos, 0);
			sim->stateMachine.ChangeState(IdleState::Instance());
		}
		return true;
	}
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
		// 집 주변까지 이동 + 집 안으로 이동
		TargetInfo targetInfo;
		targetInfo.pos = Vector2D(sim->home->m_info.m_xPos, sim->home->m_info.m_zPos);
		targetInfo.posOffset = sim->home->m_collider.getMaxBound() * 2.0f;

		sim->targetPos.push_back(targetInfo);
		sim->stateMachine.PushState(MoveState::Instance());

		sim->targetPos.emplace_back(Vector2D(sim->home->m_info.m_xPos, sim->home->m_info.m_zPos), 0);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	return false;
};