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

	int delay = rand() % 10;

	timer_event ev = { sim->id, SIM_Move, high_resolution_clock::now() + milliseconds(delay), sim->id };
	timer.add_event(ev);
};

void IdleState::Execute(Sim* sim)
{

};

void IdleState::Exit(Sim* sim)
{

};

bool IdleState::OnMessage(Sim* sim, const SIM_Message& telegram)
{
	switch (telegram.msg)
	{
	case SIM_Move:
	{
		Vector2D targetPos;
		do
		{
			targetPos = Vector2D(sim->pos.x - (rand() % 16) + 8, sim->pos.z - (rand() % 16) + 8);
		} while (PathFinder::Instance()->terrainData->extraData[(int)targetPos.x + (int)targetPos.z * 1000].collision);

		sim->targetPos.emplace_back(targetPos, 0);
		sim->stateMachine.ChangeState(MoveState::Instance());
		return true;
	}
	case SIM_Sleep:
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
	case SIM_Build:
	{
		sim->targetPos.clear();
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extra_info);

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
	
	if (distance < 0.35f)
	{
		sim->path.pop_front();
	}

	if(!sim->path.empty())
		PathFinder::Instance()->MoveToDestination(sim->path.front(), sim, 1.0f);
};

void MoveState::Exit(Sim* sim)
{
	if (sim->stateMachine.HaveNextState() == false)
		sim->stateMachine.PushState(IdleState::Instance());
};

bool MoveState::OnMessage(Sim* sim, const SIM_Message& telegram)
{
	switch (telegram.msg)
	{
	case SIM_Move:
		return true;
	case SIM_Sleep:
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
	case SIM_Build:
	{
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extra_info);

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
	timer_event ev{ sim->id, SIM_WakeUp, high_resolution_clock::now() + seconds(5), sim->id, NULL };
	timer.add_event(ev);
};

void SleepState::Execute(Sim* sim)
{

};

void SleepState::Exit(Sim* sim)
{
};

bool SleepState::OnMessage(Sim* sim, const SIM_Message& telegram)
{
	switch (telegram.msg)
	{
	case SIM_Move:
		return true;
	case SIM_Build:
	{
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extra_info);

		sim->targetPos.emplace_back(info->pos, contents.collider_info[sim->buildInfo.buildingType][sim->buildInfo.buildingIndex].getMaxBound() + 1);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());
		return true;
	}
	case SIM_WakeUp:
	{
		if (sim->stateMachine.HaveNextState())
			sim->stateMachine.ChangeState();
		else
		{
			Vector2D targetPos;
			do
			{
				targetPos = Vector2D(sim->pos.x - (rand() % 16) + 8, sim->pos.z - (rand() % 16) + 8);
			} while (terrain_data->extraData[(int)targetPos.x + (int)targetPos.z * 1000].collision);
			sim->targetPos.emplace_back(targetPos, 0);
			sim->stateMachine.ChangeState(IdleState::Instance());
		}
		return true;
	}
	}
	return false;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Build : 건설 중
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
	contents.do_construct(-1, sim->buildInfo.buildingType, sim->buildInfo.buildingIndex, sim->buildInfo.pos.x, sim->buildInfo.pos.z, 0.f);
	
	// 다음 상태로 전이
	if (sim->stateMachine.HaveNextState())
		sim->stateMachine.ChangeState();
	else
		sim->stateMachine.ChangeState(IdleState::Instance());
};

void BuildState::Exit(Sim* sim)
{
};

bool BuildState::OnMessage(Sim* sim, const SIM_Message& telegram)
{
	switch (telegram.msg)
	{
	case SIM_Sleep:
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