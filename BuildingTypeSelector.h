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
	BuildingSelector* addBuildingType(BuildingBuilder::BuildingType type, std::wstring name, float x, float y)
	{
		auto buildingTypeButton = gameObject->AddChildUI(Scene::scene->CreateImagePrefab());
		auto buildingSelectorObject = gameObject->AddChildUI(Scene::scene->CreateImagePrefab());
		{
			auto rt = buildingTypeButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0);
			rt->setPosAndSize(x, y, 40, 40);
			{
				auto textObject = buildingTypeButton->AddChildUI();
				auto rt = textObject->GetComponent<RectTransform>();
				rt->anchorMin = { 0, 0 };
				rt->anchorMax = { 1, 1 };

				Text* text = textObject->AddComponent<Text>();
				text->text = name;
				text->fontSize = 10;
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				Scene::scene->textObjects.push_back(textObject);
			}
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
};