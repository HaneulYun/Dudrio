#pragma once
#include "..\CyanEngine\framework.h"

class BuildingSelector : public MonoBehavior<BuildingSelector>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	std::vector<GameObject*> prefabs;

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingSelector>;
	BuildingSelector() = default;
	BuildingSelector(BuildingSelector&) = default;

public:
	~BuildingSelector() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
	void AddPrefab(GameObject* prefab)
	{
		prefabs.push_back(prefab);
	}
};