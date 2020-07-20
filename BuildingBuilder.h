#pragma once
#include "..\CyanEngine\framework.h"


class BuildingBuilder : public MonoBehavior<BuildingBuilder>
{
private /*�� ������ private ������ �����ϼ���.*/:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Count };

	std::vector<GameObject*> building[BuildingType::Count];

public  /*�� ������ public ������ �����ϼ���.*/:

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingBuilder>;
	BuildingBuilder() = default;
	BuildingBuilder(BuildingBuilder&) = default;

public:
	~BuildingBuilder() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
	void serializeBuildings()
	{
		building[Landmark].push_back(ASSET PREFAB("Well"));
	}
};