#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"

class CharacterController : public MonoBehavior<CharacterController>
{
private /*�� ������ private ������ �����ϼ���.*/:
	Animator* anim{ nullptr };

public  /*�� ������ public ������ �����ϼ���.*/:
	char name[MAX_ID_LEN];
	float speed = 0.0f;
	float hori_speed = 0.0f;

private:
	friend class GameObject;
	friend class MonoBehavior<CharacterController>;
	CharacterController() = default;
	CharacterController(CharacterController&) = default;

public:
	~CharacterController() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		anim = gameObject->GetComponent<Animator>();
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
		
		speed = x - curPos.x;
		hori_speed = z - curPos.z;
		if (fabs(speed) < FLT_EPSILON)	speed = 0.0f;
		if (fabs(hori_speed) < FLT_EPSILON)	hori_speed = 0.0f;

		gameObject->transform->position = { x, curPos.y, z };
		anim->SetFloat("Speed", speed);
		anim->SetFloat("HoriSpeed", hori_speed);
		gameObject->NumFramesDirty = NUM_FRAME_RESOURCES;
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};