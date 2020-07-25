#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include <chrono>
#include "MagentaServer\protocol.h"
#include "GuestNetwork.h"

class CharacterController : public MonoBehavior<CharacterController>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	Vector3 lastMousePos;

	Vector3 moveSum{ 0,0,0 };
	float lastMoveSum;
	float rotAngleSum;
	float runLevelSum;
	int averageCount;
	chrono::high_resolution_clock::time_point last_packet_time;

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
		last_packet_time = chrono::high_resolution_clock::now();
		lastMoveSum = 0.0f;
		rotAngleSum = 0.0f;
		moveSum = { 0,0,0 };
		runLevelSum = 0.0f;
		averageCount = 0;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		Vector3 tmpVel = { 0,0,0 };
		if (Input::GetKey(KeyCode::W))
			tmpVel += gameObject->transform->forward;
		if (Input::GetKey(KeyCode::A))
			tmpVel -= gameObject->transform->right;
		if (Input::GetKey(KeyCode::S))
			tmpVel -= gameObject->transform->forward;
		if (Input::GetKey(KeyCode::D))
			tmpVel += gameObject->transform->right;

		tmpVel = tmpVel.Normalize();

		if (!IsZero(tmpVel.Length())) {
			runLevelSum += 2.0f;
			if (Input::GetKey(KeyCode::Q))	// 달리기
				runLevelSum += 2.0f;
			if (Input::GetKey(KeyCode::E))	// 느리게 걷기
				runLevelSum -= 1.0f;
			moveSum += tmpVel;
		}

		averageCount++;

		if (Input::GetMouseButtonDown(2))
		{
			lastMousePos = Input::mousePosition;
		}
		else if (Input::GetMouseButton(2))
		{
			Vector3 currMousePos = Input::mousePosition;
			float rotateAngle = (Input::mousePosition.x - lastMousePos.x) * Time::deltaTime * 15.0f;

			gameObject->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, rotateAngle);

			rotAngleSum += rotateAngle;
			lastMousePos = Input::mousePosition;
		}

		if(GuestNetwork::network->isConnect && chrono::high_resolution_clock::now() - last_packet_time >= chrono::milliseconds(333))
		{
			if (IsZero(lastMoveSum) && !IsZero(moveSum.Length())) {
				GuestNetwork::network->send_move_start_packet(moveSum.x, moveSum.z, rotAngleSum, runLevelSum / averageCount);
				last_packet_time = chrono::high_resolution_clock::now();
			}
			else if (!IsZero(rotAngleSum) || !IsZero(moveSum.Length())){
				GuestNetwork::network->send_move_packet(moveSum.x, moveSum.z, rotAngleSum, runLevelSum / averageCount);
				last_packet_time = chrono::high_resolution_clock::now();
			}
			lastMoveSum = moveSum.Length();
			rotAngleSum = 0.0f;
			moveSum = { 0,0,0 };
			runLevelSum = 0.0f;
			averageCount = 0;
		}
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};