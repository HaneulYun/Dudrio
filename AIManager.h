#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*�� ������ private ������ �����ϼ���.*/:
	int simIndex = 0;
	float time = 0.0f;

public  /*�� ������ public ������ �����ϼ���.*/:
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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);

	int AddSim(GameObject* landmark, GameObject* house);

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};