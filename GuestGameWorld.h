#pragma once
#include "..\CyanEngine\framework.h"

class GuestGameWorld : public MonoBehavior<GuestGameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Lighting };
	enum TimeSpeed { X1 = 1, X2 = 2, X4 = 4, X8 = 8 };

private /*이 영역에 private 변수를 선언하세요.*/:
	const int timeOfDay = 60 * 15;

public  /*이 영역에 public 변수를 선언하세요.*/:
	static GuestGameWorld* gameWorld;

	// 랜드마크에 따른 빌딩 목록
	std::map<GameObject*, std::map<BuildingType, std::vector<GameObject*>>> buildingList;
	// 전체 심 목록
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

	void Start(/*초기화 코드를 작성하세요.*/);
	void Update(/*업데이트 코드를 작성하세요.*/);
	void gameTimeUpdate(); 
	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
	void calculateSunInfo();

	std::wstring convertTimeToText();
};