#pragma once
#include "..\CyanEngine\framework.h"

enum ButtonType { none = -1, LandMark, House, Theme, landscape, Decoration, Delete };

struct ButtonAssetItem
{
	std::wstring name;
	void(*func)(void*);
};

class ButtonManager : public MonoBehavior<ButtonManager>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	static ButtonManager* buttonManager;

	std::vector<GameObject*> buttons_BuildingType;
	std::vector<GameObject*> buttons_BuildingList[6];
	GameObject* buttons_page[2];

	std::vector<ButtonAssetItem> buttonAssetItem;

	ButtonType buttonType = none;
	int currentPage = 0;

private:
	friend class GameObject;
	friend class MonoBehavior<ButtonManager>;
	ButtonManager() = default;
	ButtonManager(ButtonManager&) = default;

public:
	~ButtonManager() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		int buildType = 4;
		int buildPage = 0;
		int buildCountInPage = 0;
		for (auto& item : buttonAssetItem)
		{
			if (!buildCountInPage)
				buttons_BuildingList[buildType].push_back(((TerrainScene*)gameObject->scene)->CreateButtonList());
			buttons_BuildingList[buildType][buildPage]->children[buildCountInPage]->children[0]->GetComponent<Text>()->text = item.name;
			buttons_BuildingList[buildType][buildPage]->children[buildCountInPage]->AddComponent<Button>()->AddEvent(item.func);

			if (++buildCountInPage == 10)
				++buildPage, buildCountInPage = 0;

		}
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
	}

	void SelectButton(ButtonType type)
	{
		if (buttonType == type)
		{
			buttons_BuildingList[type][currentPage]->SetActive(false);
			buttons_page[0]->SetActive(false); buttons_page[1]->SetActive(false);
			buttonType = none;
			currentPage = 0;
		}
		else if(type != ButtonType::Delete)
		{
			if (buttonType != none)
				buttons_BuildingList[buttonType][currentPage]->SetActive(false);
			currentPage = 0;
			buttonType = type;
			buttons_BuildingList[type][currentPage]->SetActive(true);
			buttons_page[0]->SetActive(true); buttons_page[1]->SetActive(true);
		}
	}

	void NextPage()
	{
		int pageNum = buttons_BuildingList[buttonType].size();
		if (currentPage < pageNum - 1)
		{
			buttons_BuildingList[buttonType][currentPage]->SetActive(false);
			buttons_BuildingList[buttonType][++currentPage]->SetActive(true);
		}
	}

	void PreviousPage()
	{
		if (currentPage > 0)
		{
			buttons_BuildingList[buttonType][currentPage]->SetActive(false);
			buttons_BuildingList[buttonType][--currentPage]->SetActive(true);
		}
	}
};