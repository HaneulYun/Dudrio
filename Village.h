#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class Village : public MonoBehavior<Village>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	// ���� �ֹ� ���
	std::vector<GameObject*> sims;

	// �ڵ� ���� ����
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

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};