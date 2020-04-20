#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class RotatingBehavior : public MonoBehavior<RotatingBehavior>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	float speedRotating;

private:
	friend class GameObject;
	friend class MonoBehavior<RotatingBehavior>;
	RotatingBehavior() = default;
	RotatingBehavior(RotatingBehavior&) = default;

public:
	~RotatingBehavior() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		gameObject->transform->Rotate({ 0, 1, 0 }, speedRotating* Time::deltaTime);
		gameObject->NumFramesDirty = NUM_FRAME_RESOURCES;
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};