#pragma once
#pragma once
#include "..\CyanEngine\framework.h"

class Building : public MonoBehavior<Building>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	float positionToAnimate = 0.0f;
	

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
		if (!IsZero(positionToAnimate))
		{
			gameObject->transform->position += {0.0f, 0.5f * Time::deltaTime, 0.0f};
			if (gameObject->transform->position.y > positionToAnimate)
			{
				gameObject->transform->position.y = positionToAnimate;
				positionToAnimate = 0.0f;
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
	
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};