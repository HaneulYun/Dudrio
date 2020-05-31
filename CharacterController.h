#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"
#include "GuestNetwork.h"

class CharacterController : public MonoBehavior<CharacterController>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	Vector3 lastMousePos;
	bool pressW{ false };
	bool pressS{ false };
	bool pressA{ false };
	bool pressD{ false };

public  /*이 영역에 public 변수를 선언하세요.*/:
private:
	friend class GameObject;
	friend class MonoBehavior<CharacterController>;
	CharacterController() = default;
	CharacterController(CharacterController&) = default;

public:
	~CharacterController() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
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

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};