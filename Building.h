#pragma once
#pragma once
#include "..\CyanEngine\framework.h"

class Building : public MonoBehavior<Building>
{
private /*�� ������ private ������ �����ϼ���.*/:
	float time{ 0.0f };
public  /*�� ������ public ������ �����ϼ���.*/:
	Vector3 positionToAnimate{ 0.0f,0.0f,0.0f };

	GameObject* landmark;
	int type;
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
			gameObject->transform->position += {0.0f, 0.5f * HostGameWorld::gameWorld->gameDeltaTime, 0.0f};
			time += HostGameWorld::gameWorld->gameDeltaTime;
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
		//Debug::Log("Ʈ���ſ���\n");
		if (collision->children.front()->GetComponent<Constant>())
		{
			for (auto& child : collision->children)
				child->GetComponent<Constant>()->v4 = { 0.02, 0.62, 0.75, 1 };
		}
	}

	void OnTriggerExit(GameObject* collision) 
	{
		//Debug::Log("Ʈ�����ͽ�Ʈ\n");
		if (collision->children.front()->GetComponent<Constant>() && collision->collisionType.size() < 2)
		{
			for (auto& child : collision->children)
				child->GetComponent<Constant>()->v4 = { 0.70, 0.75, 0.31, 1 };
		}
	}
	
	void setBuildingInform(GameObject* landmark, int type, int index)
	{
		this->landmark = landmark;
		this->type = type;
		this->index = index;
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};