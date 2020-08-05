#pragma once
#include "..\CyanEngine\framework.h"
#include "GuestUI.h"

class GuestGameWorld : public MonoBehavior<GuestGameWorld, GameWorld>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	static GuestGameWorld* gameWorld;

	GuestUI* guestUI;

private:
	friend class GameObject;
	friend class MonoBehavior<GuestGameWorld, GameWorld>;
	GuestGameWorld() = default;
	GuestGameWorld(GuestGameWorld&) = default;

public:
	~GuestGameWorld() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);
	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);
	void uiUpdate();
	void gameTimeUpdate(); 
	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);
};