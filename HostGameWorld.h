#pragma once
#include "..\CyanEngine\framework.h"

class HostGameWorld : public MonoBehavior<HostGameWorld, GameWorld>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	static HostGameWorld* gameWorld;

	GameObject*	simPrefab;
	int			simIndex = 0;

	/*Time*/
	int				day = 1;

private:
	friend class GameObject;
	friend class MonoBehavior<HostGameWorld, GameWorld>;
	HostGameWorld() = default;
	HostGameWorld(HostGameWorld&) = default;

public:
	~HostGameWorld() {}

	void Start(/*초기화 코드를 작성하세요.*/);

	void Update(/*업데이트 코드를 작성하세요.*/);

	void aiUpdate();

	void gameTimeUpdate();

	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index);

	int addSim(GameObject* landmark, GameObject* house);

	int eraseSim(GameObject* landmark, GameObject* house);
};