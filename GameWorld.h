#pragma once
#include "..\CyanEngine\framework.h"

class GameWorld : public MonoBehavior<GameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Lighting };
	enum TimeSpeed { X1 = 1, X2 = 2, X4 = 4 };
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	static GameWorld* gameWorld;

	// 랜드마크에 따른 빌딩 목록
	std::map<GameObject*, std::map<BuildingType, std::vector<GameObject*>>> buildingList;
	// 전체 심 목록
	std::map<int, GameObject*> simList;


	float gameTime;
	float gameDeltaTime;


	int gameMoney;


	int simIndex = 0;
	GameObject* simPrefab;
private:
	friend class GameObject;
	friend class MonoBehavior<GameWorld>;
	GameWorld() = default;
	GameWorld(GameWorld&) = default;

public:
	~GameWorld() {}

	void Start(/*초기화 코드를 작성하세요.*/);

	void Update(/*업데이트 코드를 작성하세요.*/);

	void aiUpdate();

	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	int addSim(GameObject* landmark, GameObject* house);

	int eraseSim(GameObject* landmark, GameObject* house);
};