#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"
#include "GuestNetwork.h"

class CharacterController : public MonoBehavior<CharacterController>
{
private /*�� ������ private ������ �����ϼ���.*/:
	Vector3 lastMousePos;
	bool pressW{ false };
	bool pressS{ false };
	bool pressA{ false };
	bool pressD{ false };

public  /*�� ������ public ������ �����ϼ���.*/:
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
		if (Input::GetKey(KeyCode::W)) pressW = true;
		else pressW = false;

		if (Input::GetKey(KeyCode::S)) pressS = true;
		else pressS = false;

		if (Input::GetKey(KeyCode::D)) pressD = true;
		else pressD = false;
		
		if (Input::GetKey(KeyCode::A)) pressA = true;
		else pressA = false;

		Vector3 tmpVelocity = { 0,0,0 };

		if (pressW)	tmpVelocity += gameObject->transform->forward;
		if (pressS) tmpVelocity -= gameObject->transform->forward;
		if (pressD)	tmpVelocity += gameObject->transform->right;
		if (pressA) tmpVelocity -= gameObject->transform->right;

		if (GuestNetwork::network->isConnect)
		{
			CharacterMovingBehavior* mymovebhvr = gameObject->GetComponent<CharacterMovingBehavior>();
			if (IsZero(tmpVelocity.Length()) && mymovebhvr->moving)
			{
				GuestNetwork::network->send_move_end_packet();
				mymovebhvr->moving = false;
			}
			else if (!IsZero(tmpVelocity.Length()) && !mymovebhvr->moving)
			{
				tmpVelocity = tmpVelocity.Normalize();
				GuestNetwork::network->send_move_start_packet(gameObject->transform->position.x, gameObject->transform->position.z, tmpVelocity.x, tmpVelocity.z);
				mymovebhvr->moving = true;
			}
		}

		if (Input::GetMouseButtonDown(2))
		{
			lastMousePos = Input::mousePosition;
		}
		else if (Input::GetMouseButton(2))
		{
			Vector3 currMousePos = Input::mousePosition;

			gameObject->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, (Input::mousePosition.x - lastMousePos.x) * Time::deltaTime);

			lastMousePos = Input::mousePosition;
		}
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};