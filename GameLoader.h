#pragma once
#include "..\CyanEngine\framework.h"
#include <fstream>

class GameLoader : public MonoBehavior<GameLoader>
{
private /*�� ������ private ������ �����ϼ���.*/:
	
public  /*�� ������ public ������ �����ϼ���.*/:
	static GameLoader* gameLoader;

private:
	friend class GameObject;
	friend class MonoBehavior<GameLoader>;
	GameLoader() = default;
	GameLoader(GameLoader&) = default;

public:
	~GameLoader() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		
	}

	void Save()
	{
		std::ofstream out("Buildings.txt");
		//for (auto& b : BuildManager::buildManager->buildings)
		//	out << b.first;
		out.close();
	}

	void Load()
	{
		//for (auto& b : BuildManager::buildManager->buildings)
		//{
		//	Scene::scene->PushDelete(b.second);
		//}
		//BuildManager::buildManager->buildings.clear();

		//if (HostNetwork::network != nullptr)
		//{
		//	if (HostNetwork::network->isConnect)
		//	{
		//		HostNetwork::network->send_destruct_all_packet();
		//	}
		//}
		//
		//std::ifstream in("Buildings.txt");
		//istream_iterator<BuildingInform> beg{ in };
		//istream_iterator<BuildingInform> end{};
		//
		//while (beg != end)
		//{
		//	//auto iter = BuildManager::buildManager->buildings.find(*beg);
		//	//
		//	//if (iter == BuildManager::buildManager->buildings.end())
		//	//{
		//	//	Builder::builder->BuildNewBuilding(*beg);
		//	//}
		//	*beg++;
		//}
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};