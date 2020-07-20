#pragma once
#include "..\CyanEngine\framework.h"


class BuildingBuilder : public MonoBehavior<BuildingBuilder>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Count };

	std::vector<GameObject*> building[BuildingType::Count];

public  /*이 영역에 public 변수를 선언하세요.*/:

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingBuilder>;
	BuildingBuilder() = default;
	BuildingBuilder(BuildingBuilder&) = default;

public:
	~BuildingBuilder() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
	void serializeBuildings()
	{
		building[Landmark].push_back(ASSET PREFAB("Well"));
	}
};