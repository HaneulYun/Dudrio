#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class CharacterMovingBehavior : public MonoBehavior<CharacterMovingBehavior>
{
private /*�� ������ private ������ �����ϼ���.*/:


public  /*�� ������ public ������ �����ϼ���.*/:
	Animator* anim{ nullptr };
	char name[MAX_ID_LEN];
	Vector3 velocity{ 0,0,0 };
	Vector3 accel{ 0,0,0 };

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
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		// �������� ���� ��ġ �ε巴�� �����ϴ� �� �ۼ��ϱ�
		gameObject->transform->position += velocity * Time::deltaTime;
		gameObject->transform->position.y = heightmap->GetHeight(gameObject->transform->position.x, gameObject->transform->position.z);

		anim->SetFloat("VelocityZ", velocity.z);
		anim->SetFloat("VelocityX", velocity.x);
	}

	void move(Vector3 newPos)
	{
		gameObject->transform->position = newPos;
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};