#pragma once
#include "..\CyanEngine\framework.h"

class GameWorld : public MonoBehavior<GameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Lighting };
	enum TimeSpeed { X1 = 1, X2 = 2, X4 = 4 };
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	static GameWorld* gameWorld;

	// ���帶ũ�� ���� ���� ���
	std::map<GameObject*, std::map<BuildingType, std::vector<GameObject*>>> buildingList;
	// ��ü �� ���
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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);

	void aiUpdate();

	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	int addSim(GameObject* landmark, GameObject* house);

	int eraseSim(GameObject* landmark, GameObject* house);
};