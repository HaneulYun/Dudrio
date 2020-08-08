#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class GameUI : public MonoBehavior<GameUI>
{
public :
	enum GameUICategory{UIBar, MenuUI, TimeX1, TimeX2, TimeX4, LandMarkUI, ChatUI, SimCountUI, CoinCountUI, DayAndTimeUI, SelectFrame, FPS, Count};
private /*이 영역에 private 변수를 선언하세요.*/:
public  /*이 영역에 public 변수를 선언하세요.*/:
	std::vector<GameObject*> gameUIs;


private:
	friend class GameObject;
	friend class MonoBehavior<GameUI>;
	GameUI() = default;
	GameUI(GameUI&) = default;

public:
	~GameUI() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		gameUIs.resize(GameUICategory::Count);

		gameUIs[ChatUI] = Scene::scene->CreateImage();
		{
			auto rt = gameUIs[ChatUI]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 0);
			rt->setPosAndSize(0, 0, 400, 25);

			auto chatField = gameUIs[ChatUI]->AddComponent<InputField>();
			auto text = chatField->Text();
			text->fontSize = 12;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			gameUIs[ChatUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_chatInput");
		}
		gameUIs[ChatUI]->SetActive(false);

		gameUIs[SimCountUI] = Scene::scene->CreateImage();
		{
			auto rt = gameUIs[SimCountUI]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(1, 0);
			rt->setPosAndSize(-320, 0, 120, 25);

			Text* text = gameUIs[SimCountUI]->AddComponent<Text>();
			text->text = L"100";
			text->fontSize = 12;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			gameUIs[SimCountUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_sim");
		}

		gameUIs[CoinCountUI] = Scene::scene->CreateImage();
		{
			auto rt = gameUIs[CoinCountUI]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(1, 0);
			rt->setPosAndSize(-200, 0, 120, 25);

			Text* text = gameUIs[CoinCountUI]->AddComponent<Text>();
			text->text = L"100000";
			text->fontSize = 12;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			gameUIs[CoinCountUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_coin");
		}

		gameUIs[DayAndTimeUI] = Scene::scene->CreateImage();
		{
			auto rt = gameUIs[DayAndTimeUI]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(1, 0);
			rt->setPosAndSize(0, 0, 240, 25);

			Text* text = gameUIs[DayAndTimeUI]->AddComponent<Text>();
			text->fontSize = 12;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			gameUIs[DayAndTimeUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_time");
		}

		gameUIs[SelectFrame] = Scene::scene->CreateImage();
		{
			auto rt = gameUIs[SelectFrame]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0);
			rt->setPosAndSize(-140, 0, 40, 40);

			gameUIs[SelectFrame]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_selectFrame");
		}
		gameUIs[SelectFrame]->SetActive(false);


		gameUIs[FPS] = Scene::scene->CreateUI();
		{
			auto rt = gameUIs[FPS]->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 1);

			Text* text = gameUIs[FPS]->AddComponent<Text>();
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->color = { 1,1,1,1 };
		}

	

	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};