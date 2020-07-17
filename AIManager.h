#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	float time = 0.0f;
	bool b = false;
public  /*이 영역에 public 변수를 선언하세요.*/:
	std::vector<Village*> villages;

private:
	friend class GameObject;
	friend class MonoBehavior<AIManager>;
	AIManager() = default;
	AIManager(AIManager&) = default;

public:
	~AIManager() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
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

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};