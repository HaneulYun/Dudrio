#pragma once
#include "CyanEngine\CyanEngine\framework.h"

// 랜드마크가 가지는 스크립트

class Village : public MonoBehavior<Village>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	//		Home			Sim
	std::map<GameObject*, GameObject*> simList;

	float delayTime = 0.f;
	bool autoDevelopment = false;

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
		
	}

	void OnAutoDevelopment()
	{
		autoDevelopment = true;
		delayTime = rand() % 10 + 10;
	}

	void OffAutoDevelopment()
	{
		autoDevelopment = false;
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};