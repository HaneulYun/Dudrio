#pragma once
#include "..\CyanEngine\framework.h"

enum ButtonType { none = 0, LandMark, House, Theme, landscape, Decoration };

class ButtonManager : public MonoBehavior<ButtonManager>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	static ButtonManager* buttonManager;

	std::vector<std::pair<GameObject*, bool>> buttons;

	ButtonType buttonType;

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

	void SelectButton(int buttonIndex)
	{
		buttons[buttonIndex].second ? DisableButton(buttonIndex) : EnableButton(buttonIndex);
	}

	void EnableButton(int buttonIndex)
	{
		buttons[buttonIndex].first->SetActive(true);
		buttons[buttonIndex].second = true;
	}

	void DisableButton(int buttonIndex)
	{
		//gameObject->scene->PushDisable(buttons[buttonIndex].first);
		buttons[buttonIndex].first->SetActive(false);
		buttons[buttonIndex].second = false;
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};