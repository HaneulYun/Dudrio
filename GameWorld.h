#pragma once
#include "..\CyanEngine\framework.h"

class GameWorld : public MonoBehavior<GameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Lighting, Fence, Prop, Count, None = -1 };
	enum TimeSpeed { X1 = 1, X2 = 2, X4 = 4, X8 = 8 };
private /*�� ������ private ������ �����ϼ���.*/:
	const int timeOfDay = 60 * 15;

public  /*�� ������ public ������ �����ϼ���.*/:
	static GameWorld* gameWorld;

	// ���帶ũ�� ���� ���� ���
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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);

	void aiUpdate();

	void gameTimeUpdate();

	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	int addSim(GameObject* landmark, GameObject* house);

	int eraseSim(GameObject* landmark, GameObject* house);

	void calculateSunInfo();

	std::wstring convertTimeToText();
};