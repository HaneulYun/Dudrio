#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class HostInformConnector : public MonoBehavior<HostInformConnector>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	InputField* nameField{ nullptr };

	InputField* frequencyField{ nullptr };
	InputField* octavesField{ nullptr };
	InputField* seedField{ nullptr };

	GameObject* nameGuide{ nullptr };

	GameObject* frequencyGuide{ nullptr };
	GameObject* octavesGuide{ nullptr };
	GameObject* seedGuide{ nullptr };

	GameObject* prevButton{ nullptr };
	GameObject* button{ nullptr };	// 뒤로가기 버튼

public  /*이 영역에 public 변수를 선언하세요.*/:
	GameObject* newVillageButton{ nullptr };
	GameObject* loadVillageButton{ nullptr };

	// 마을 주인 이름
	static char name[MAX_ID_LEN + 1];

	// 게임 시간
	float ingame_time;
	// 지형 정보
	static float terrainSize; // = 1000;
	static int frequency;	// 0 ~ 64
	static int octaves;	// 0 ~ 16
	static int seed;	// ~4byte

	// 건물 정보

	static HostInformConnector* connector;
private:
	friend class GameObject;
	friend class MonoBehavior<HostInformConnector>;
	HostInformConnector() = default;
	HostInformConnector(HostInformConnector&) = default;

public:
	~HostInformConnector() {}

	bool checkRangeInField()
	{
		if (nameField->text.length() > MAX_ID_LEN || nameField->text.length() < 2)
			return false;
		if (frequencyField->text.empty())
			return false;
		if (frequencyField->text.length() <= 2) {
			int ifrequency;
			std::wstringstream(frequencyField->text) >> ifrequency;
			if (ifrequency >= 64 || ifrequency < 0)
				return false;
		}
		else return false;
		if (octavesField->text.empty())
			return false;
		if (octavesField->text.length() <= 2) {
			int ioctaves;
			std::wstringstream(octavesField->text) >> ioctaves;
			if (ioctaves >= 16 || ioctaves < 0)
				return false;
		}
		else return false;
		if (seedField->text.length() > 9 || octavesField->text.empty())
			return false;
		else {
			int iseeds;
			std::wstringstream(seedField->text) >> iseeds;
			if (iseeds < 0)
				return false;
		}
		return true;
	}

	void clearFields()
	{
		nameField->clear();
		frequencyField->clear();
		octavesField->clear();
		seedField->clear();
	}

	bool insertInform()
	{
		if (!checkRangeInField())
			return false;
		std::string sname;
		sname.assign(nameField->text.begin(), nameField->text.end());
		strncpy(name, sname.c_str(), sname.length());
		std::wstringstream(frequencyField->text) >> frequency;
		std::wstringstream(octavesField->text) >> octaves;
		std::wstringstream(seedField->text) >> seed;

		clearFields();

		return true;
	}

	void backToPrevPage()
	{
		newVillageButton->SetActive(true);
		loadVillageButton->SetActive(true);
		clearFields();
		SetField(false);
	}

	void goToNextPage()
	{
		newVillageButton->SetActive(false);
		loadVillageButton->SetActive(false);
		clearFields();
		SetField(true);
	}

	void SetField(bool flag)
	{
		nameField->gameObject->SetActive(flag);
		frequencyField->gameObject->SetActive(flag);
		octavesField->gameObject->SetActive(flag);
		seedField->gameObject->SetActive(flag);

		nameGuide->SetActive(flag);
		frequencyGuide->SetActive(flag);
		octavesGuide->SetActive(flag);
		seedGuide->SetActive(flag);

		button->SetActive(flag);
		prevButton->SetActive(flag);
	}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		prevButton = Scene::scene->CreateImage();
		{
			auto rt = prevButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(30, -50, 40, 40);

			prevButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					connector->backToPrevPage();
				});
			{
				auto textobject = prevButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"<-";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		nameGuide = Scene::scene->CreateUI();
		{
			auto rt = nameGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -50, 300, 40);

			Text* text = nameGuide->AddComponent<Text>();
			text->text = L"Input your name";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto nameFieldObject = Scene::scene->CreateImage();
		{
			auto rt = nameFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -100, 300, 40);

			nameField = nameFieldObject->AddComponent<InputField>();
			auto text = nameField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		frequencyGuide = Scene::scene->CreateUI();
		{
			auto rt = frequencyGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -200, 1000, 40);

			Text* text = frequencyGuide->AddComponent<Text>();
			text->text = L"Input the frequency of your village's terrain (0 ~ 64)";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto frequencyFieldObject = Scene::scene->CreateImage();
		{
			auto rt = frequencyFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -250, 300, 40);

			frequencyField = frequencyFieldObject->AddComponent<InputField>();
			auto text = frequencyField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		octavesGuide = Scene::scene->CreateUI();
		{
			auto rt = octavesGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -350, 1000, 40);

			Text* text = octavesGuide->AddComponent<Text>();
			text->text = L"Input the octaves of your village's terrain (0 ~ 16)";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto octavesFieldObject = Scene::scene->CreateImage();
		{
			auto rt = octavesFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -400, 300, 40);

			octavesField = octavesFieldObject->AddComponent<InputField>();
			auto text = octavesField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		seedGuide = Scene::scene->CreateUI();
		{
			auto rt = seedGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -500, 1000, 40);

			Text* text = seedGuide->AddComponent<Text>();
			text->text = L"Input the seed of your village's terrain (0 ~ MAX_INT)";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto seedFieldObject = Scene::scene->CreateImage();
		{
			auto rt = seedFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(100, -550, 300, 40);

			seedField = seedFieldObject->AddComponent<InputField>();
			auto text = seedField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		button = Scene::scene->CreateImage();
		{
			auto rt = button->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(700, -700, 380, 40);

			button->AddComponent<Button>()->AddEvent(
				[](void*) {
					if (connector->insertInform())
						SceneManager::LoadScene("HostScene");
					else
						Debug::Log("범위 초과");
					connector->clearFields();
				});
			{
				auto textobject = button->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Set my village's information";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}
		SetField(false);
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{


	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};