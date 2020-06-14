#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class CharacterMovingBehavior : public MonoBehavior<CharacterMovingBehavior>
{
private /*�� ������ private ������ �����ϼ���.*/:


public  /*�� ������ public ������ �����ϼ���.*/:
	Animator* anim{ nullptr };
	char name[MAX_ID_LEN];
	Vector3 velocity{ 0,0,0 };

	bool moving{ false };
	bool rotate{ false };
	float newRotAngle;
	float curRotAngle;
	TerrainData* heightmap = NULL;
private:
	friend class GameObject;
	friend class MonoBehavior<CharacterMovingBehavior>;
	CharacterMovingBehavior() = default;
	CharacterMovingBehavior(CharacterMovingBehavior&) = default;

public:
	~CharacterMovingBehavior() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		newRotAngle = 0.0f;
		curRotAngle = 0.0f;
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		// �������� ���� ��ġ �ε巴�� �����ϴ� �� �ۼ��ϱ�
		if (moving)
		{
			gameObject->transform->position.x += velocity.x * Time::deltaTime;
			gameObject->transform->position.z += velocity.z * Time::deltaTime;
		}
		if (rotate)
		{
			float angle = newRotAngle * Time::deltaTime / 0.333;
			gameObject->transform->Rotate(Vector3{ 0,1,0 }, angle);
			curRotAngle += angle;
			if (fabs(curRotAngle) >= fabs(newRotAngle))
			{
				gameObject->transform->Rotate(Vector3{ 0,1,0 }, curRotAngle - newRotAngle);
				rotate = false;
				curRotAngle = 0.0f;
				newRotAngle = 0.0f;
			}
		}
		gameObject->transform->position.y = heightmap->GetHeight(gameObject->transform->position.x, gameObject->transform->position.z);
		anim->SetFloat("VelocityZ", velocity.z);
		anim->SetFloat("VelocityX", velocity.x);
	}

	void move(float xPos, float zPos)
	{
		gameObject->transform->position.x = xPos;
		gameObject->transform->position.y = heightmap->GetHeight(xPos, zPos);
		gameObject->transform->position.z = zPos;
	}

	void turn(float destAngle)
	{
		newRotAngle += destAngle;
		rotate = true;
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};