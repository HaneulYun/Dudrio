#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class GuestController : public MonoBehavior<GuestController>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	Vector3 velocity{ 0, 0, 0 };
	Vector3 accel{ 0, 0, 0 };

public  /*이 영역에 public 변수를 선언하세요.*/:
	Animator* animator;

private:
	friend class GameObject;
	friend class MonoBehavior<GuestController>;
	GuestController() = default;
	GuestController(GuestController&) = default;

public:
	~GuestController() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		if(gameObject->children.size())
			gameObject->children[0]->NumFramesDirty = NUM_FRAME_RESOURCES;

		accel = { 0, 0, 0 };
		if (Input::GetKey(KeyCode::W))
			accel += gameObject->transform->forward * 2 * Time::deltaTime;
		if (Input::GetKey(KeyCode::S))
			accel -= gameObject->transform->forward * 2 * Time::deltaTime;
		if (Input::GetKey(KeyCode::D))
			accel += gameObject->transform->right * 2 * Time::deltaTime;
		if (Input::GetKey(KeyCode::A))
			accel -= gameObject->transform->right * 2 * Time::deltaTime;

		velocity += accel;
		if (velocity.Length() > 1.0f)
			velocity.Normalize();

		if (accel == Vector3{ 0, 0, 0 })
			velocity -= velocity * 1.5f * Time::deltaTime;

		gameObject->transform->position += velocity * Time::deltaTime;

		animator->SetFloat("VelocityZ", velocity.z);
		animator->SetFloat("VelocityX", velocity.x);
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};