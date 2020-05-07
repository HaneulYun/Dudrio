#pragma once
#pragma once
#include "..\CyanEngine\framework.h"

class Building : public MonoBehavior<Building>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	float positionToAnimate = 0.0f;
	

private:
	friend class GameObject;
	friend class MonoBehavior<Building>;
	Building() = default;
	Building(Building&) = default;

public:
	~Building() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
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
	
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};