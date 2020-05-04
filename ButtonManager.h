#pragma once
#include "..\CyanEngine\framework.h"

enum ButtonType { none = -1, LandMark, House, Theme, landscape, Decoration, Delete };

class ButtonManager : public MonoBehavior<ButtonManager>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	static ButtonManager* buttonManager;

	std::vector<GameObject*> buttons_BuildingType;
	std::vector<GameObject*> buttons_BuildingList[6];
	GameObject* buttons_page[2];

	ButtonType buttonType = none;
	int currentPage = 0;

private:
	friend class GameObject;
	friend class MonoBehavior<ButtonManager>;
	ButtonManager() = default;
	ButtonManager(ButtonManager&) = default;

public:
	~ButtonManager() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
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