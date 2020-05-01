#pragma once
#include "..\CyanEngine\framework.h"

class ButtonManager : public MonoBehavior<ButtonManager>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	static ButtonManager* buttonManager;

	std::vector<std::pair<GameObject*, bool>> buttons;

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
		gameObject->scene->AddGameObject(buttons[buttonIndex].first);
		buttons[buttonIndex].second = true;
	}

	void DisableButton(int buttonIndex)
	{
		gameObject->scene->PushDisable(buttons[buttonIndex].first);
		buttons[buttonIndex].second = false;
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};