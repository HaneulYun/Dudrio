#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class Village : public MonoBehavior<Village>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	// 마을 주민 목록
	std::vector<GameObject*> sims;

	// 자동 발전 여부
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

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};