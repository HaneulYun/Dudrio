#pragma once
#include "..\CyanEngine\framework.h"

class GameWorld : public MonoBehavior<GameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Lighting, Fence, Prop, Count, None = -1 };
	enum TimeSpeed { X1 = 1, X2 = 2, X4 = 4, X8 = 8 };
private /*이 영역에 private 변수를 선언하세요.*/:
	const int timeOfDay = 60 * 15;

public  /*이 영역에 public 변수를 선언하세요.*/:
	static GameWorld* gameWorld;

	// 랜드마크에 따른 빌딩 목록
	std::map<GameObject*, std::map<BuildingType, std::vector<GameObject*>>> buildingList;

	std::map<int, GameObject*>	simList;
	GameObject*	simPrefab;
	int			simIndex = 0;

	/*Time*/
	GameObject*		sun;
	TimeSpeed		timeSpeed = X1;

	int				day = 1;
	float			gameTime = 37.5f * 20;
	float			gameDeltaTime;

	/*money*/
	int				gameMoney;
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

	void gameTimeUpdate();

	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	int addSim(GameObject* landmark, GameObject* house);

	int eraseSim(GameObject* landmark, GameObject* house);

	void calculateSunInfo();

	std::wstring convertTimeToText();
};