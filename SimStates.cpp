#include "pch.h"
#include "SimStates.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////                                                       
// Idle : ������ �ִ� ��
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
	{
		Vector2 targetPos;
		do
		{
			targetPos = Vector2(sim->gameObject->transform->position.x - (rand() % 16) + 8, sim->gameObject->transform->position.z - (rand() % 16) + 8);
		} while (PathFinder::Instance()->terrainNodeData->extraData[(int)targetPos.x + (int)targetPos.y * 1024].collision);

		sim->targetPos.emplace_back(targetPos, 0);
		sim->stateMachine.ChangeState(MoveState::Instance());
		return true;
	}
	case Msg_Sleep:
	{
		sim->stateMachine.ClearStack();
		sim->targetPos.clear();
		sim->path.clear();

		// �� �ֺ����� �̵� + �� ������ �̵�
		TargetInfo targetInfo;
		targetInfo.pos = Vector2(sim->home->transform->position.x, sim->home->transform->position.z);
		targetInfo.posOffset = sim->home->GetComponent<BoxCollider>()->extents.y * 2.0f;

		sim->targetPos.push_back(targetInfo);
		sim->stateMachine.PushState(MoveState::Instance());

		sim->targetPos.emplace_back(Vector2(sim->home->transform->position.x, sim->home->transform->position.z), 0);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	case Msg_Build:
	{
		sim->targetPos.clear();
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);

		sim->targetPos.emplace_back(info->pos, BuildingBuilder::buildingBuilder->getBoundingBox(sim->buildInfo.buildingType, sim->buildInfo.buildingIndex) + 1);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	}
	return false;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Move : �����̴� ��
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


	if (PathFinder::Instance()->FindPath(sim->targetPos.front().pos, Vector2(sim->gameObject->transform->position.x, sim->gameObject->transform->position.z), sim->path, checkCollisionOn, targetPosOffset) == false)
	{
		if (sim->stateMachine.HaveNextState() == false)
			sim->stateMachine.PushState(IdleState::Instance());
		sim->stateMachine.ChangeState();
	}
};

void MoveState::Execute(Sim* sim)
{
	sim->animator->SetFloat("Walking", 2);

	if (sim->path.empty())
	{
		sim->targetPos.pop_front();

		sim->stateMachine.ChangeState();
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

		// �� �ֺ����� �̵� + �� ������ �̵�
		TargetInfo targetInfo;
		targetInfo.pos = Vector2(sim->home->transform->position.x, sim->home->transform->position.z);
		targetInfo.posOffset = sim->home->GetComponent<BoxCollider>()->extents.y * 2.0f;

		sim->targetPos.push_back(targetInfo);
		sim->stateMachine.PushState(MoveState::Instance());

		sim->targetPos.emplace_back(Vector2(sim->home->transform->position.x, sim->home->transform->position.z), 0);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	case Msg_Build:
	{
		BuildMessageInfo* info = static_cast<BuildMessageInfo*>(telegram.extraInfo);

		sim->targetPos.emplace_back(info->pos, BuildingBuilder::buildingBuilder->getBoundingBox(sim->buildInfo.buildingType, sim->buildInfo.buildingIndex) + 1);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(BuildState::Instance());
	}
		return true;
	}
	return false;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Sleep : �ڴ� ��
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
	sim->animator->SetFloat("Walking", 0);

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

		sim->targetPos.emplace_back(info->pos, BuildingBuilder::buildingBuilder->getBoundingBox(sim->buildInfo.buildingType, sim->buildInfo.buildingIndex) + 1);
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
			Vector2 targetPos;
			do
			{
				targetPos = Vector2(sim->gameObject->transform->position.x - (rand() % 16) + 8, sim->gameObject->transform->position.z - (rand() % 16) + 8);
			} while (BuildingBuilder::buildingBuilder->terrainNodeData->extraData[(int)targetPos.x + (int)targetPos.y * 1024].collision);
			sim->targetPos.emplace_back(targetPos, 0);
			sim->stateMachine.ChangeState(IdleState::Instance());
		}
		return true;
	}
	}
	return false;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Sleep : �Ǽ� ��
/////////////////////////////////////////////////////////////////////////////////////////////////////
BuildState* BuildState::Instance()
{
	static BuildState instance;

	return &instance;
}

void BuildState::Enter(Sim* sim)
{
	sim->animator->SetFloat("Walking", 0);
};

void BuildState::Execute(Sim* sim)
{
	// �ǹ� �Ǽ�
	BuildingBuilder::buildingBuilder->build(sim->buildInfo.pos, 0, sim->buildInfo.buildingType, sim->buildInfo.buildingIndex, sim->home->GetComponent<Building>()->landmark);

	// ���� ���·� ����
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
		// �� �ֺ����� �̵� + �� ������ �̵�
		TargetInfo targetInfo;
		targetInfo.pos = Vector2(sim->home->transform->position.x, sim->home->transform->position.z);
		targetInfo.posOffset = sim->home->GetComponent<BoxCollider>()->extents.y * 2.0f;

		sim->targetPos.push_back(targetInfo);
		sim->stateMachine.PushState(MoveState::Instance());

		sim->targetPos.emplace_back(Vector2(sim->home->transform->position.x, sim->home->transform->position.z), 0);
		sim->stateMachine.PushState(MoveState::Instance());
		sim->stateMachine.PushState(SleepState::Instance());

		sim->stateMachine.ChangeState();
		return true;
	}
	return false;
};