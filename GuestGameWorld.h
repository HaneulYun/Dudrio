#pragma once
#include "..\CyanEngine\framework.h"

class GuestGameWorld : public MonoBehavior<GuestGameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Lighting };

private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	static GuestGameWorld* gameWorld;

	// 랜드마크에 따른 빌딩 목록
	std::map<BuildingType, std::vector<GameObject*>> buildingList;
	// 전체 심 목록
	std::map<int, GameObject*> simList;

	float gameTime;
	float gameDeltaTime;

	int simIndex = 0;
	GameObject* simPrefab;

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
	void buildInGameWorld(GameObject* building, int type, int index);
	void deleteInGameWorld(GameObject* building, int type, int index);
};