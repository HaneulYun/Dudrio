#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"
#include "GuestNetwork.h"

class CharacterController : public MonoBehavior<CharacterController>
{
private /*�� ������ private ������ �����ϼ���.*/:


public  /*�� ������ public ������ �����ϼ���.*/:
	GuestNetwork* network;

private:
	friend class GameObject;
	friend class MonoBehavior<CharacterController>;
	CharacterController() = default;
	CharacterController(CharacterController&) = default;

public:
	~CharacterController() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		CharacterMovingBehavior* myMoveFunc = gameObject->GetComponent<CharacterMovingBehavior>();
		myMoveFunc->accel = { 0,0,0 };
		if (Input::GetKey(KeyCode::W))
		{
			myMoveFunc->accel += gameObject->transform->forward * 2 * Time::deltaTime;
		}
		if (Input::GetKey(KeyCode::S))
		{
			myMoveFunc->accel -= gameObject->transform->forward * 2 * Time::deltaTime;
		}
		if (Input::GetKey(KeyCode::D))
		{
			myMoveFunc->accel += gameObject->transform->right * 2 * Time::deltaTime;
		}
		if (Input::GetKey(KeyCode::A))
		{
			myMoveFunc->accel -= gameObject->transform->right * 2 * Time::deltaTime;
		}
	
		myMoveFunc->velocity += myMoveFunc->accel;
		if (myMoveFunc->velocity.Length() > 1.0f)
			myMoveFunc->velocity.Normalize();

		if(myMoveFunc->accel == Vector3{0,0,0})
			myMoveFunc->velocity -= myMoveFunc->velocity * 1.5f * Time::deltaTime;

		if (!IsZero(myMoveFunc->velocity.Length()))
		{
			network->send_move_packet(myMoveFunc->velocity.x, myMoveFunc->velocity.z);
			myMoveFunc->move();
		}
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};