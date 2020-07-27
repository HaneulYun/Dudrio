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


	//		landmark
	std::map<GameObject*, std::map<BuildingType, std::vector<GameObject*>>> buildingList;
	
	// ���帶ũ�� ���� �� ���?
	std::map<int, Sim*> simList;
	

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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		//aiUpdate();
		
		for (auto landmark : buildingList)
		{
			for (auto sim : landmark.first->GetComponent<Village>()->sims)
			{

			}
			for (auto house : landmark.second[House])
			{
				
			}
			for (auto light : landmark.second[Lighting])
			{
			}
		}
	}

	void aiUpdate()
	{
		if (simList.empty())
			return;

		gameTime += Time::deltaTime;

		if (gameTime > 60.f)
		{
			for (auto s : simList)
			{
				Messenger->CreateMessage(0, s.first, s.first, Msg_Sleep);
			}
			gameTime -= 60.f;
		}

		Messenger->Timer();
	}

	void buildInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
	{
		if (type == BuildingType::Prop)
		{
			if (building->GetComponent<Light>())
				type = BuildingType::Lighting;
		}
		buildingList[landmark][(BuildingType)type].push_back(building);
		
		if (type == BuildingType::House)
			addSim(landmark, building);
	}

	void deleteInGameWorld(GameObject* landmark, GameObject* building, int type, int index)
	{
		if (type == BuildingType::Prop)
		{
			if (building->GetComponent<Light>())
				type = BuildingType::Lighting;
		}
		buildingList[landmark][(BuildingType)type].erase(find(buildingList[landmark][(BuildingType)type].begin(), buildingList[landmark][(BuildingType)type].end(), building));

		if (type == BuildingType::House)
		{
			eraseSim(landmark, building);
		}
	}

	int addSim(GameObject* landmark, GameObject* house)
	{
		GameObject* sim = Scene::scene->Duplicate(simPrefab);
		sim->transform->position = house->transform->position;

		auto simComponent = sim->AddComponent<Sim>();
		simComponent->animator = sim->children[0]->GetComponent<Animator>();
		simComponent->home = house;
		simComponent->id = simIndex;
		simList[simIndex++] = simComponent;

		// ��Ʈ��ũ ���� �ȵǾ�������
		simComponent->stateMachine.PushState(IdleState::Instance());
		simComponent->stateMachine.GetCurrentState()->Enter(simComponent);

		landmark->GetComponent<Village>()->sims[house] = sim;

		return simIndex;
	}

	int eraseSim(GameObject* landmark, GameObject* house)
	{
		GameObject* sim = landmark->GetComponent<Village>()->sims[house];

		landmark->GetComponent<Village>()->sims.erase(house);
		simList.erase(sim->GetComponent<Sim>()->id);
	}
};