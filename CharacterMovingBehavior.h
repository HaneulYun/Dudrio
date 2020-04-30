#pragma once
#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include "MagentaServer\protocol.h"

class CharacterMovingBehavior : public MonoBehavior<CharacterMovingBehavior>
{
private /*�� ������ private ������ �����ϼ���.*/:


public  /*�� ������ public ������ �����ϼ���.*/:
	Animator* anim{ nullptr };
	char name[MAX_ID_LEN];
	Vector3 velocity{ 0,0,0 };
	Vector3 accel{ 0,0,0 };

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
	}

	void move(Vector3 newPos)
	{
		gameObject->transform->position = newPos;
		anim->SetFloat("VelocityZ", velocity.z);
		anim->SetFloat("VelocityX", velocity.x);
	}

	void move()
	{
		gameObject->transform->position += velocity * Time::deltaTime;
		anim->SetFloat("VelocityZ", velocity.z);
		anim->SetFloat("VelocityX", velocity.x);
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};