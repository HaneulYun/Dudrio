#pragma once
#include "..\CyanEngine\framework.h"

class BuildingTypeSelector : public MonoBehavior<BuildingTypeSelector>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	std::vector<BuildingSelector*> buildingSelectors;

	BuildingBuilder* builder{ nullptr };

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingTypeSelector>;
	BuildingTypeSelector() = default;
	BuildingTypeSelector(BuildingTypeSelector&) = default;

public:
	~BuildingTypeSelector() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
	BuildingSelector* addBuildingType(BuildingBuilder::BuildingType type, float x, float y, Material* material)
	{
		auto buildingTypeButton = gameObject->AddChildUI(Scene::scene->CreateImagePrefab());
		buildingTypeButton->GetComponent<Renderer>()->materials[0] = material;
		auto buildingSelectorObject = gameObject->AddChildUI(Scene::scene->CreateImagePrefab());
		{
			auto rt = buildingTypeButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0);
			rt->setPosAndSize(x, y, 40, 40);

			buildingTypeButton->AddComponent<Button>()->AddEvent([](void* ptr)
				{
					auto object = reinterpret_cast<GameObject*>(ptr);
					object->parent->GetComponent<BuildingTypeSelector>()->InactivateChildren(object);
					object->SetActive(!object->active);
					if(object->active)
						object->GetComponent<BuildingSelector>()->setBuildingButtonName();
				}, buildingSelectorObject);
		}

		{
			buildingSelectorObject->GetComponent<RectTransform>()->setAnchorAndPivot(0.5, 0);
			buildingSelectorObject->GetComponent<RectTransform>()->setPosAndSize(0, 60, 510, 60);
			buildingSelectorObject->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("gray");
		}
		auto buildingSelector = buildingSelectorObject->AddComponent<BuildingSelector>();
		buildingSelector->type = type;
		buildingSelector->buildingBuilder = builder;

		buildingSelectors.push_back(buildingSelector);

		return buildingSelector;
	}

	void InactivateChildren(GameObject* exception = nullptr)
	{
		for (auto type : buildingSelectors)
			if (auto children = type->gameObject; children != exception)
				children->SetActive(false);
	}

	void addDeleteButton(float x, float y, Material* material)
	{
		auto DeleteButton = gameObject->AddChildUI(Scene::scene->CreateImagePrefab());
		DeleteButton->GetComponent<Renderer>()->materials[0] = material;
		{
			auto rt = DeleteButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0);
			rt->setPosAndSize(x, y, 40, 40);

			DeleteButton->AddComponent<Button>()->AddEvent([](void* ptr)
				{
					BuildingBuilder::buildingBuilder->enterDeleteMode();
				});
		}
	}
};