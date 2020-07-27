#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class AIManager : public MonoBehavior<AIManager>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	static AIManager* aiManager;

private:
	friend class GameObject;
	friend class MonoBehavior<AIManager>;
	AIManager() = default;
	AIManager(AIManager&) = default;

public:
	~AIManager() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);

	void aiUpdate();

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};