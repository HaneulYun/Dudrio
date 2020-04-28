#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class GuestController : public MonoBehavior<GuestController>
{
private /*�� ������ private ������ �����ϼ���.*/:
	Vector3 velocity{ 0, 0, 0 };
	Vector3 accel{ 0, 0, 0 };

public  /*�� ������ public ������ �����ϼ���.*/:
	Animator* animator;

private:
	friend class GameObject;
	friend class MonoBehavior<GuestController>;
	GuestController() = default;
	GuestController(GuestController&) = default;

public:
	~GuestController() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
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

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};