#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include <unordered_set>

struct RoomInfo {
	int room_id;
	char name[MAX_ID_LEN + 1];
	std::string ip;
	int port_num;
	int terrain_size;
	int frequency;
	int octaves;
	int seed;
};

class GuestInformConnector : public MonoBehavior<GuestInformConnector>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	// page 1
	InputField* nameField{ nullptr };
	GameObject* nameGuide{ nullptr };

	InputField* lobbyServerIPField{ nullptr };
	GameObject* lobbyServerIPGuide{ nullptr };

	GameObject* characterCustomizePrevButton{ nullptr };
	GameObject* characterCustomizeNextButton{ nullptr };

	GameObject* SetMyCharacterbutton{ nullptr };

	// page 2
	GameObject* prevButton{ nullptr };	// 뒤로가기 버튼
	std::unordered_map<int, std::pair<GameObject*, RoomInfo>> hosts;

public  /*이 영역에 public 변수를 선언하세요.*/:
	GameObject* background{ nullptr };
	GameObject* buttonPrefab{ nullptr };

	// 게스트 이름
	char name[MAX_ID_LEN + 1];
	std::string lobbyServerIP;

	// 지형 정보
	float terrainSize = 1000;
	int frequency;	// 0 ~ 64
	int octaves;	// 0 ~ 16
	int seed;	// ~4byte

	static GuestInformConnector* connector;
private:
	friend class GameObject;
	friend class MonoBehavior<GuestInformConnector>;
	GuestInformConnector() = default;
	GuestInformConnector(GuestInformConnector&) = default;

public:
	~GuestInformConnector() {}

	bool checkRangeInField()
	{
		if (nameField->text.length() > MAX_ID_LEN || nameField->text.length() < 2)
			return false;
		return true;
	}

	void clearFields()
	{
		nameField->clear();
		lobbyServerIPField->clear();
	}

	bool insertInform()
	{
		if (!checkRangeInField())
			return false;
		std::string sname;
		sname.assign(nameField->text.begin(), nameField->text.end());
		strncpy(name, sname.c_str(), sname.length());

		lobbyServerIP.assign(lobbyServerIPField->text.begin(), lobbyServerIPField->text.end());

		clearFields();
		return true;
	}

	void backToPrevPage()
	{
		background->GetComponent<RectTransform>()->anchorMin = { 0.5, 0 };
		SetField(true);
		clearFields();
		
		for (auto host : hosts)
			Scene::scene->PushDelete(host.second.first);
		hosts.clear();
		// closesocket 하자
	}

	void goToNextPage()
	{
		SetField(false);
		clearFields();
		//GameObject* g = gameObject->scene->Duplicate(buttonPrefab);
		//g->layer = (int)RenderLayer::UI;
		hosts[0] = std::make_pair(gameObject->scene->Duplicate(buttonPrefab), RoomInfo{});
		background->GetComponent<RectTransform>()->anchorMin = { 0, 0 };
	}

	void SetField(bool flag)
	{
		nameField->gameObject->SetActive(flag);
		nameGuide->SetActive(flag);

		lobbyServerIPField->gameObject->SetActive(flag);
		lobbyServerIPGuide->SetActive(flag);

		characterCustomizePrevButton->SetActive(flag);
		characterCustomizeNextButton->SetActive(flag);

		SetMyCharacterbutton->SetActive(flag);

		// ----------------------------------------

		prevButton->SetActive(!flag);
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
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		characterCustomizePrevButton = Scene::scene->CreateImage();
		{
			auto rt = characterCustomizePrevButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(30, -380, 40, 40);

			characterCustomizePrevButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					// 커스터마이징 버튼
				});
			{
				auto textobject = characterCustomizePrevButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"<-";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		characterCustomizeNextButton = Scene::scene->CreateImage();
		{
			auto rt = characterCustomizeNextButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(530, -380, 40, 40);

			characterCustomizeNextButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					// 커스터마이징 버튼
				});
			{
				auto textobject = characterCustomizeNextButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"->";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		nameGuide = Scene::scene->CreateUI();
		{
			auto rt = nameGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -50, 300, 40);

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
			rt->setPosAndSize(650, -100, 300, 40);

			nameField = nameFieldObject->AddComponent<InputField>();
			auto text = nameField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		lobbyServerIPGuide = Scene::scene->CreateUI();
		{
			auto rt = lobbyServerIPGuide->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -350, 500, 40);

			Text* text = lobbyServerIPGuide->AddComponent<Text>();
			text->text = L"Input lobby server's IP";
			text->fontSize = 30;
			text->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		auto lobbyServerIPFieldObject = Scene::scene->CreateImage();
		{
			auto rt = lobbyServerIPFieldObject->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(650, -400, 300, 40);

			lobbyServerIPField = lobbyServerIPFieldObject->AddComponent<InputField>();
			auto text = lobbyServerIPField->Text();
			text->fontSize = 30;
			text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}

		SetMyCharacterbutton = Scene::scene->CreateImage();
		{
			auto rt = SetMyCharacterbutton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0, 1);
			rt->setPosAndSize(970, -700, 200, 70);

			SetMyCharacterbutton->AddComponent<Button>()->AddEvent(
				[](void*) {
					if (connector->insertInform()) {
						// 서버 연결 후 다음 페이지로
						connector->goToNextPage();
					}
				});
			{
				auto textobject = SetMyCharacterbutton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Set my character";
				text->fontSize = 30;
				text->color = { 0.0f, 0.0f, 0.0f, 1.0f };
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}
		SetField(true);
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{


	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};