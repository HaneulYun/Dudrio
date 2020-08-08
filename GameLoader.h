#pragma once
#include "..\CyanEngine\framework.h"
#include <fstream>
#include <io.h>
#include "BuildingBuilder.h"
#include "HostGameWorld.h"

struct BuildingFormat {
	int b_type;
	int b_index;
	double x, z;
	double angle;
	int radius;
	bool development;

	std::string makeInfoToString()
	{
		std::string info;
		info = std::to_string(b_type) + ' ' + std::to_string(b_index) + ' ' + std::to_string(x) + ' ' + std::to_string(z)
			+ ' ' + std::to_string(angle) + ' ' + std::to_string(radius) + ' ' + std::to_string(development);
		
		return info;
	}

	std::string makeInfoToString2()
	{
		std::string info;
		info = std::to_string(b_type) + ' ' + std::to_string(b_index) + ' ' + std::to_string(x) + ' ' + std::to_string(z);

		return info;
	}
};

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
		gameLoader = this;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		
	}

	void insertInFile(int b_type, int b_idx, double x, double z, double angle, int range, bool development)
	{
		BuildingFormat b_info{ b_type, b_idx, x, z, angle, range, development };
		std::string infoToString = b_info.makeInfoToString();
		
		std::fstream file("buildings.txt", std::ios::in | std::ios::out | std::ios::app);
		if(file.is_open() == false)
			file.open("buildings.txt", std::ios::in | std::ios::out | std::ios::trunc);

		if (file.is_open()) {
			file << infoToString << std::endl;
		}
	}

	void initFile(std::wstring name, int frequency, int octaves, int seed)
	{
		std::fstream file("buildings.txt", std::ios::out);
		std::string str;
		str.assign(name.begin(), name.end());

		file << str << ' ' << frequency << ' ' << octaves << ' ' << seed << std::endl;
		file.close();
	}

	void deleteInFile(int b_type, int b_idx, double x, double z, double angle, int range)
	{
		BuildingFormat b_info{ b_type, b_idx, x, z, angle, range };
		std::string infoToString = b_info.makeInfoToString2();

		FILE* fp = fopen("buildings.txt", "rt+");
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

	void SaveTime(float& gametime, int& gameday)
	{
		std::fstream time("time.txt", std::ios::out);
		time << gametime << ' ' << gameday;
		time.close();
	}

	int Load(std::wstring& name, int& frequency, int& octaves, int& seed, float& gametime, int& gameday)
	{
		std::fstream file("buildings.txt", std::ios::in);
		if (!file) {
			file.close();
			std::fstream file2("buildings.txt", std::ios::out);
			file2 << "Dudrio 3 3 1" << std::endl;
			file2.close();
			file.open("buildings.txt", std::ios::in);
		}

		std::string str;
		file >> str >> frequency >> octaves >> seed;
		name.assign(str.begin(), str.end());

		std::fstream time("time.txt", std::ios::in);
		if (!time) {
			time.close();
			std::fstream file2("time.txt", std::ios::out);
			file2 << "750.0 1" << std::endl;
			file2.close();
			time.open("time.txt", std::ios::in);
		}
		time >> gametime >> gameday;

		return file.tellp();
	}
	
	void Save(std::wstring name, int frequency, int octaves, int seed)
	{
		std::fstream file("buildings.txt", std::ios::out);
		std::string str;
		str.assign(name.begin(), name.end());
		file << str << ' ' << frequency << ' ' << octaves << ' ' << seed << std::endl;
		BuildingFormat bf;

		for (auto& landmark : HostGameWorld::gameWorld->buildingList) {
			if (landmark.first == nullptr)
				continue;
			bf.b_type = BuildingBuilder::Landmark;
			bf.b_index = landmark.first->GetComponent<Building>()->index;
			bf.x = landmark.first->transform->position.x;
			bf.z = landmark.first->transform->position.z;
			Vector3 building_forward = landmark.first->transform->forward;
			building_forward.y = 0;
			building_forward.Normalize();
			Vector3 forward = { 0,0,1 };
			double angle = Vector3::DotProduct(forward, building_forward);
			Vector3 dir = Vector3::CrossProduct(forward, building_forward);
			angle = XMConvertToDegrees(acos(angle));
			angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;
			bf.angle = angle;
			bf.radius = landmark.first->GetComponent<Village>()->radiusOfLand;
			bf.development = landmark.first->GetComponent<Village>()->autoDevelopment;
			file << bf.makeInfoToString() << std::endl;
		}

		for (auto& landmark : HostGameWorld::gameWorld->buildingList) {
			for (auto& type : landmark.second) {
				bf.b_type = type.first;
				for (auto& building : type.second) {
					if (building->GetComponent<Village>() != nullptr)
						continue;
					bf.b_index = building->GetComponent<Building>()->index;
					bf.x = building->transform->position.x;
					bf.z = building->transform->position.z;
					Vector3 building_forward = building->transform->forward;
					building_forward.y = 0;
					building_forward.Normalize();
					Vector3 forward = { 0,0,1 };
					double angle = Vector3::DotProduct(forward, building_forward);
					Vector3 dir = Vector3::CrossProduct(forward, building_forward);
					angle = XMConvertToDegrees(acos(angle));
					angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;
					bf.angle = angle;
					bf.radius = 0;
					bf.development = false;
					file << bf.makeInfoToString() << std::endl;
				}
			}
		}
		file.close();
	}

	void LoadBuildings(int pfile)
	{
		std::fstream file("buildings.txt", std::ios::in);
		file.seekp(pfile, std::ios::beg);

		int type, index;
		float x, z, angle;
		int range;
		bool development;
		while (file >> type >> index >> x >> z >> angle >> range >> development) {
			BuildingBuilder::buildingBuilder->hostLoad(type, index, x, z, angle, range, development);
		}
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};