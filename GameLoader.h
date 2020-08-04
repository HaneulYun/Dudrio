#pragma once
#include "..\CyanEngine\framework.h"
#include <fstream>
#include <io.h>

struct BuildingFormat {
	int b_type;
	int b_index;
	float x, z;
	float angle;
	int radius;

	std::string makeInfoToString()
	{
		std::string info;
		info = to_string(b_type) + ' ' + to_string(b_index) + ' ' + to_string(x) + ' ' + to_string(z) 
			+ ' ' + to_string(angle) + ' ' + to_string(radius);
		
		std::wstring winfo;
		winfo.assign(info.begin(), info.end());
		winfo += L"\n";
		Debug::Log(winfo.c_str());

		return info;
	}
};

class GameLoader : public MonoBehavior<GameLoader>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	std::string filename{ "buildings.txt" };
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
		gameLoader = this;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		
	}

	void insertInFile(int b_type, int b_idx, float x, float z, float angle, int range)
	{
		BuildingFormat b_info{ b_type, b_idx, x, z, angle, range };
		string infoToString = b_info.makeInfoToString();
		
		fstream file(filename, ios::in | ios::out | ios::app);
		if(file.is_open() == false)
			file.open(filename, ios::in | ios::out | ios::trunc);

		if (file.is_open()) {
			file << infoToString << endl;
		}
	}

	void modifyLandmarkRange(int b_type, int b_idx, float x, float z, float angle, int prev_range, int new_range)
	{

	}

	void deleteInFile(int b_type, int b_idx, float x, float z, float angle, int range)
	{
		BuildingFormat b_info{ b_type, b_idx, x, z, angle, range };
		string infoToString = b_info.makeInfoToString();

		FILE* fp = fopen(filename.c_str(), "rt+");
		char buf[256];

		rewind(fp);
		{
			long seek, start;
			while (1)
			{
				seek = ftell(fp);
				if (fgets(buf, 256, fp) == NULL) break;
				if (strstr(buf, infoToString.c_str()) != NULL) {
					start = seek;
					long len = _filelength(_fileno(fp)) - ftell(fp);
					char* tmp = (char*)malloc(len);
					len = fread(tmp, 1, len, fp);

					fseek(fp, start, SEEK_SET);
					fwrite(tmp, 1, len, fp);
					fflush(fp);
					free(tmp);
					_chsize(_fileno(fp), ftell(fp));
					break;
				}

			}
		}
		rewind(fp);
	}

	void Save()
	{
		//std::ofstream out("Buildings.txt");
		//for (auto& b : GameWorld::gameWorld->buildingList) {
		//
		//}
		//	out << b.first;
		//out.close();
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