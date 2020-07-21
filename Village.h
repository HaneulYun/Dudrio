#pragma once
#include "CyanEngine\CyanEngine\framework.h"

// ���帶ũ�� ������ ��ũ��Ʈ

class Village : public MonoBehavior<Village>
{
private /*�� ������ private ������ �����ϼ���.*/:
	bool autoDevelopment = false;
	float timeDelay = 0.f;

public  /*�� ������ public ������ �����ϼ���.*/:
	// ���� �ֹ� ���
	std::vector<GameObject*> sims;

private:
	friend class GameObject;
	friend class MonoBehavior<Village>;
	Village() = default;
	Village(Village&) = default;

public:
	~Village() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		if (autoDevelopment)
		{
			if (timeDelay <= 0.f)
			{
				BuildMessageInfo* info = new BuildMessageInfo;
				info->pos = Vector2(gameObject->transform->position.x + (rand() % 30) - 15, gameObject->transform->position.z + (rand() % 30) - 15);
				info->type = BuildingType::House_01;
				info->simList = &sims;

				//autoDevelopment = false;

				timeDelay = rand() % 10 + 10;
				Messenger->CreateMessage(0, -1, -1, Msg_Build, info);
			}
			
			timeDelay -= Time::deltaTime;
		}
	}

	void OnAutoDevelopment()
	{
		autoDevelopment = true;
		timeDelay = rand() % 10 + 10;
	}

	void OffAutoDevelopment()
	{
		autoDevelopment = false;
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};