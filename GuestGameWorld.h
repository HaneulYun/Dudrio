#pragma once
#include "..\CyanEngine\framework.h"

class GuestGameWorld : public MonoBehavior<GuestGameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Lighting };
	enum TimeSpeed { X1 = 1, X2 = 2, X4 = 4, X8 = 8 };

private /*�� ������ private ������ �����ϼ���.*/:
	const int timeOfDay = 60 * 15;

public  /*�� ������ public ������ �����ϼ���.*/:
	static GuestGameWorld* gameWorld;

	// ���帶ũ�� ���� ���� ���
	std::map<GameObject*, std::map<BuildingType, std::vector<GameObject*>>> buildingList;
	// ��ü �� ���
	std::map<int, GameObject*> simList;

	/*Time*/
	GameObject* sun;
	TimeSpeed		timeSpeed = X1;
	float			gameTime = 37.5f * 6;
	float			gameDeltaTime;

	/*money*/
	int				gameMoney;

private:
	friend class GameObject;
	friend class MonoBehavior<GuestGameWorld>;
	GuestGameWorld() = default;
	GuestGameWorld(GuestGameWorld&) = default;

public:
	~GuestGameWorld() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);
	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);
	void gameTimeUpdate(); 
	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
	void calculateSunInfo();

	std::wstring convertTimeToText();
};