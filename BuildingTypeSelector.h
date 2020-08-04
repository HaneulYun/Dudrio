#pragma once
#include "..\CyanEngine\framework.h"

class BuildingTypeSelector : public MonoBehavior<BuildingTypeSelector>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	std::vector<BuildingSelector*> buildingSelectors;

	BuildingBuilder* builder{ nullptr };

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingTypeSelector>;
	BuildingTypeSelector() = default;
	BuildingTypeSelector(BuildingTypeSelector&) = default;

public:
	~BuildingTypeSelector() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
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