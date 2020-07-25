#pragma once
#include "..\CyanEngine\framework.h"
#include <fstream>

class GameLoader : public MonoBehavior<GameLoader>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	
public  /*이 영역에 public 변수를 선언하세요.*/:
	static GameLoader* gameLoader;

private:
	friend class GameObject;
	friend class MonoBehavior<GameLoader>;
	GameLoader() = default;
	GameLoader(GameLoader&) = default;

public:
	~GameLoader() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
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
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};