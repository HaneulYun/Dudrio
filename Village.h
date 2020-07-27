#pragma once
#include "CyanEngine\CyanEngine\framework.h"

// ���帶ũ�� ������ ��ũ��Ʈ

class Village : public MonoBehavior<Village>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
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

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};