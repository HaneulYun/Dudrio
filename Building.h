#pragma once
#pragma once
#include "..\CyanEngine\framework.h"

class Building : public MonoBehavior<Building>
{
private /*�� ������ private ������ �����ϼ���.*/:
	float time{ 0.0f };
public  /*�� ������ public ������ �����ϼ���.*/:
	Vector3 positionToAnimate{ 0.0f,0.0f,0.0f };
	int index;

private:
	friend class GameObject;
	friend class MonoBehavior<Building>;
	Building() = default;
	Building(Building&) = default;

public:
	~Building() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		if (!IsZero(positionToAnimate.y))
		{
			gameObject->transform->position += {0.0f, 0.5f * Time::deltaTime, 0.0f};
			time += Time::deltaTime;
			if (time >= 0.1f)
			{
				time -= 0.1f;
				gameObject->transform->position.x = positionToAnimate.x + MathHelper::RandF(-0.1f, 0.1f);
				gameObject->transform->position.z = positionToAnimate.z + MathHelper::RandF(-0.1f, 0.1f);
			}
			if (gameObject->transform->position.y > positionToAnimate.y)
			{
				gameObject->transform->position = positionToAnimate;
				positionToAnimate = { 0.0f,0.0f,0.0f };
				time = 0.0f;
			}
		}
	}

	void OnTriggerEnter(GameObject* collision) 
	{
		if (collision->GetComponent<Building>())
			for (auto& d : gameObject->children)
				d->GetComponent<Constant>()->v4 = { 1.0f,0.0f,0.0f,1.0f };
	}

	void OnTriggerExit(GameObject* collision) 
	{
		if (collision->GetComponent<Building>() && gameObject->collisionType.size() < 2)
			for (auto& d : gameObject->children)
			d->GetComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
	}
	
	void SetBuildingIndex(int idx)
	{
		index = idx;
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};