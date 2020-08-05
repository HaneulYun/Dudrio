#pragma once
#include "..\CyanEngine\framework.h"
#include "GuestUI.h"

class GuestGameWorld : public MonoBehavior<GuestGameWorld, GameWorld>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	static GuestGameWorld* gameWorld;

	GuestUI* guestUI;

private:
	friend class GameObject;
	friend class MonoBehavior<GuestGameWorld, GameWorld>;
	GuestGameWorld() = default;
	GuestGameWorld(GuestGameWorld&) = default;

public:
	~GuestGameWorld() {}

	void Start(/*초기화 코드를 작성하세요.*/);
	void Update(/*업데이트 코드를 작성하세요.*/);
	void uiUpdate();
	void gameTimeUpdate(); 
	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
};