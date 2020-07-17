#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*�� ������ private ������ �����ϼ���.*/:
	float time = 0.0f;
	bool b = false;
public  /*�� ������ public ������ �����ϼ���.*/:
	std::vector<Village*> villages;

private:
	friend class GameObject;
	friend class MonoBehavior<AIManager>;
	AIManager() = default;
	AIManager(AIManager&) = default;

public:
	~AIManager() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		time += Time::deltaTime;
		
		if (time > 60.f)
		{
			for (auto s : SimManager::Instance()->sims)
			{
				Messenger->CreateMessage(0, s.first, s.first, Msg_Sleep);
			}
			time -= 60.f;
			b = true;
		}

		Messenger->Timer();
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};