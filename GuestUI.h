#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class GuestUI : public MonoBehavior<GuestUI>
{
public:
	enum GuestUICategory { UIBar, MenuUI, ChatUI, DayAndTimeUI, WorldNameUI, Count };
private /*�� ������ private ������ �����ϼ���.*/:
public  /*�� ������ public ������ �����ϼ���.*/:
	std::vector<GameObject*> guestUIs;


private:
	friend class GameObject;
	friend class MonoBehavior<GuestUI>;
	GuestUI() = default;
	GuestUI(GuestUI&) = default;

public:
	~GuestUI() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
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
			text->font = L"����ǹ��� ����";
			text->text = L"\t ���ϴ� ���� ����";
			text->color = { 0.9140625f, 0.796875f, 0.37890625f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			guestUIs[WorldNameUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_world");
		}

	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};