#pragma once
#include "..\CyanEngine\framework.h"

enum ButtonType { none = 0, LandMark, House, Theme, landscape, Decoration };

class ButtonManager : public MonoBehavior<ButtonManager>
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
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

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
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

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};