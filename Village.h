#pragma once
#include "CyanEngine\CyanEngine\framework.h"

// ���帶ũ�� ������ ��ũ��Ʈ
enum LANDRADIUS { LAND_SMALL = 50, LAND_MEDIUM = 80, LAND_LARGE = 150 };

class Village : public MonoBehavior<Village>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	//		Home			Sim
	std::map<GameObject*, GameObject*> simList;
	std::vector<int> serverSimList;

	float delayTime = 0.f;
	bool autoDevelopment = false;

	int radiusOfLand{ LAND_SMALL };
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

	void eraseSimInServerList(int idx)
	{
		auto iter = find(serverSimList.begin(), serverSimList.end(), idx);

		if (iter != serverSimList.end())
			serverSimList.erase(iter);
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};