#pragma once
#include "..\CyanEngine\framework.h"

class GuestGameWorld : public MonoBehavior<GuestGameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Lighting };

private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	static GuestGameWorld* gameWorld;

	// ���帶ũ�� ���� ���� ���
	std::map<BuildingType, std::vector<GameObject*>> buildingList;
	// ��ü �� ���
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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);
	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);
	void gameTimeUpdate(); 
	void buildInGameWorld(GameObject* building, int type, int index);
	void deleteInGameWorld(GameObject* building, int type, int index);
};