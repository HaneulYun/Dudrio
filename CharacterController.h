#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"

class CharacterController : public MonoBehavior<CharacterController>
{
private /*�� ������ private ������ �����ϼ���.*/:


public  /*�� ������ public ������ �����ϼ���.*/:
	Animator* anim{ nullptr };
	char name[MAX_ID_LEN];
	float speed = 0.0f;
	float hori_speed = 0.0f;

	bool isShowing;

private:
	friend class GameObject;
	friend class MonoBehavior<CharacterController>;
	CharacterController() = default;
	CharacterController(CharacterController&) = default;

public:
	~CharacterController() {}

	void hide()
	{
		isShowing = false;
		gameObject->transform->position.y = -1000.0f;
	}
	
	void show()
	{
		isShowing = true;
		gameObject->transform->position.y = 0.0f;
	}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		hide();
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		//if (Input::GetKey(KeyCode::W))
		//{
		//	if (speed < 4.0f)
		//	{
		//		speed += 1.0f * Time::deltaTime;
		//		if (speed > 1.0f)
		//			speed = 1.0f;
		//	}
		//}
		//else
		//{
		//	if (speed > 0.0f)
		//	{
		//		speed -= 2.0f * Time::deltaTime;
		//		if (speed < 0.0f)
		//			speed = 0.0f;
		//	}
		//}
		//
		//if (Input::GetKey(KeyCode::S))
		//{
		//	if (speed > -4.0f)
		//	{
		//		speed -= 1.0f * Time::deltaTime;
		//		if (speed < -1.0f)
		//			speed = -1.0f;
		//	}
		//}
		//else
		//{
		//	if (speed < 0.0f)
		//	{
		//		speed += 2.0f * Time::deltaTime;
		//		if (speed > 0.0f)
		//			speed = 0.0f;
		//	}
		//}
		//
		//if (Input::GetKey(KeyCode::D))
		//{
		//	if (hori_speed < 4.0f)
		//	{
		//		hori_speed += 1.0f * Time::deltaTime;
		//		if (hori_speed > 1.0f)
		//			hori_speed = 1.0f;
		//	}
		//}
		//else
		//{
		//	if (hori_speed > 0.0f)
		//	{
		//		hori_speed -= 4.0f * Time::deltaTime;
		//		if (hori_speed < 0.0f)
		//			hori_speed = 0.0f;
		//	}
		//}
		//
		//if (Input::GetKey(KeyCode::A))
		//{
		//	if (hori_speed > -3.0f)
		//	{
		//		hori_speed -= 1.0f * Time::deltaTime;
		//		if (hori_speed < -1.0f)
		//			hori_speed = -1.0f;
		//	}
		//}
		//else
		//{
		//	if (hori_speed < 0.0f)
		//	{
		//		hori_speed += 2.0f * Time::deltaTime;
		//		if (hori_speed > 0.0f)
		//			hori_speed = 0.0f;
		//	}
		//}
		//
		//gameObject->transform->position += Vector3(hori_speed, 0, speed) * Time::deltaTime;
		//anim->SetFloat("Speed", speed);
		//anim->SetFloat("HoriSpeed", hori_speed);
		//
		//gameObject->NumFramesDirty = NUM_FRAME_RESOURCES;
	}

	void move(float x, float z)
	{
		auto curPos = gameObject->transform->position;

		if (IsZero(hori_speed))	hori_speed = 0.0f;
		if (IsZero(speed))	speed = 0.0f;

		gameObject->transform->position = { x, curPos.y, z };
		anim->SetFloat("VelocityZ", speed);
		anim->SetFloat("VelocityX", hori_speed);
	}

	void move()
	{
		gameObject->transform->position += Vector3(hori_speed, 0, speed) * Time::deltaTime;
		anim->SetFloat("VelocityZ", speed);
		anim->SetFloat("VelocityX", hori_speed);
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};