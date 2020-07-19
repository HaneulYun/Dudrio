#pragma once
#include "..\CyanEngine\framework.h"

class BuildingSelector : public MonoBehavior<BuildingSelector>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	std::vector<GameObject*> prefabs;

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingSelector>;
	BuildingSelector() = default;
	BuildingSelector(BuildingSelector&) = default;

public:
	~BuildingSelector() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
	void AddPrefab(GameObject* prefab)
	{
		prefabs.push_back(prefab);
	}
};