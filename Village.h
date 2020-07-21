#pragma once
#include "CyanEngine\CyanEngine\framework.h"

// 랜드마크가 가지는 스크립트

class Village : public MonoBehavior<Village>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	bool autoDevelopment = false;
	float timeDelay = 0.f;

public  /*이 영역에 public 변수를 선언하세요.*/:
	// 마을 주민 목록
	std::vector<GameObject*> sims;

private:
	friend class GameObject;
	friend class MonoBehavior<Village>;
	Village() = default;
	Village(Village&) = default;

public:
	~Village() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
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
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};