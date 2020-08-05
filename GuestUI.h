#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class GuestUI : public MonoBehavior<GuestUI>
{
public:
	enum GuestUICategory { UIBar, MenuUI, ChatUI, DayAndTimeUI, WorldNameUI, Count };
private /*이 영역에 private 변수를 선언하세요.*/:
public  /*이 영역에 public 변수를 선언하세요.*/:
	std::vector<GameObject*> guestUIs;


private:
	friend class GameObject;
	friend class MonoBehavior<GuestUI>;
	GuestUI() = default;
	GuestUI(GuestUI&) = default;

public:
	~GuestUI() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		guestUIs.resize(GuestUICategory::Count);

		guestUIs[ChatUI] = Scene::scene->CreateImage();
		{
			auto rt = guestUIs[ChatUI]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 0);
			rt->setPosAndSize(0, 0, 400, 25);

			auto chatField = guestUIs[ChatUI]->AddComponent<InputField>();
			auto text = chatField->Text();
			text->fontSize = 12;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			guestUIs[ChatUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_chatInput");
		}
		guestUIs[ChatUI]->SetActive(false);


		guestUIs[DayAndTimeUI] = Scene::scene->CreateImage();
		{
			auto rt = guestUIs[DayAndTimeUI]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(1, 0);
			rt->setPosAndSize(0, 0, 240, 25);

			Text* text = guestUIs[DayAndTimeUI]->AddComponent<Text>();
			text->fontSize = 12;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			guestUIs[DayAndTimeUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_time");
		}

		guestUIs[WorldNameUI] = Scene::scene->CreateImage();
		{
			auto rt = guestUIs[WorldNameUI]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(0, 0, 380, 90);

			Text* text = guestUIs[WorldNameUI]->AddComponent<Text>();
			text->fontSize = 20;
			text->font = L"배달의민족 도현";
			text->text = L"\t 윤하늘 님의 월드";
			text->color = { 0.9140625f, 0.796875f, 0.37890625f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			guestUIs[WorldNameUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_world");
		}

	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};