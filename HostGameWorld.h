#pragma once
#include "..\CyanEngine\framework.h"
#include "GameUI.h"

class HostGameWorld : public MonoBehavior<HostGameWorld, GameWorld>
{
public:
	enum GameState { CameraMode, ChatMode, MenuMode };
private /*�� ������ private ������ �����ϼ���.*/:
	
public  /*�� ������ public ������ �����ϼ���.*/:
	static HostGameWorld* gameWorld;

	GameObject*	simPrefab;
	int			simIndex = 0;

	/*Time*/
	int				day = 1;

	GameState		gameState = CameraMode;
	GameUI* gameUI;
private:
	friend class GameObject;
	friend class MonoBehavior<HostGameWorld, GameWorld>;
	HostGameWorld() = default;
	HostGameWorld(HostGameWorld&) = default;

public:
	~HostGameWorld() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);

	void aiUpdate();

	void uiUpdate();

	void gameTimeUpdate();

	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	int addSim(GameObject* landmark, GameObject* house);

	int eraseSim(GameObject* landmark, GameObject* house);

	void changeMode(GameState state);
};