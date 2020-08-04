#pragma once
#include "..\CyanEngine\framework.h"
#include <fstream>
#include <io.h>
#include "BuildingBuilder.h"

struct BuildingFormat {
	int b_type;
	int b_index;
	double x, z;
	double angle;
	int radius;

	std::string makeInfoToString()
	{
		std::string info;
		info = std::to_string(b_type) + ' ' + std::to_string(b_index) + ' ' + std::to_string(x) + ' ' + std::to_string(z)
			+ ' ' + std::to_string(angle) + ' ' + std::to_string(radius);
		
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

	void insertInFile(int b_type, int b_idx, double x, double z, double angle, int range)
	{
		BuildingFormat b_info{ b_type, b_idx, x, z, angle, range };
		std::string infoToString = b_info.makeInfoToString();
		
		std::fstream file("buildings.txt", std::ios::in | std::ios::out | std::ios::app);
		if(file.is_open() == false)
			file.open("buildings.txt", std::ios::in | std::ios::out | std::ios::trunc);

		if (file.is_open()) {
			file << infoToString << std::endl;
		}
	}

	void modifyLandmarkRange(int b_type, int b_idx, double x, double z, double angle, int prev_range, int new_range)
	{

	}

	void initFile(std::string name, int frequency, int octaves, int seed)
	{
		std::fstream file("buildings.txt", std::ios::out);
		file << name << ' ' << frequency << ' ' << octaves << ' ' << seed << std::endl;
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

	void Save()
	{
		//std::ofstream out("Buildings.txt");
		//for (auto& b : GameWorld::gameWorld->buildingList) {
		//
		//}
		//	out << b.first;
		//out.close();
	}

	int Load(std::string& name, int& frequency, int& octaves, int& seed)
	{
		std::fstream file("buildings.txt", std::ios::in);
		file >> name >> frequency >> octaves >> seed;

		return file.tellp();
	}

	void LoadBuildings(int pfile)
	{
		std::fstream file("buildings.txt", std::ios::in);
		file.seekp(pfile, std::ios::beg);

		int type, index;
		float x, z, angle;
		int range;

		while (file >> type >> index >> x >> z >> angle >> range) {
			BuildingBuilder::buildingBuilder->hostLoad(type, index, x, z, angle, range);
		}
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};