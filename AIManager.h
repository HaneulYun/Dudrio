#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	int simIndex = 0;
	float time = 0.0f;

public  /*이 영역에 public 변수를 선언하세요.*/:
	static AIManager* Instance;

	std::map<int, Sim*> simList;
	std::vector<Village*> villages;

	GameObject* simPrefab;

private:
	friend class GameObject;
	friend class MonoBehavior<AIManager>;
	AIManager() = default;
	AIManager(AIManager&) = default;

public:
	~AIManager() {}

	void Start(/*초기화 코드를 작성하세요.*/);

	void Update(/*업데이트 코드를 작성하세요.*/);

	int AddSim(GameObject* landmark, GameObject* house);

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};