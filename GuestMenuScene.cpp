#include "pch.h"
#include "GuestMenuScene.h"

GuestInformConnector* GuestInformConnector::connector{ nullptr };
char GuestInformConnector::name[MAX_ID_LEN + 1];
RoomInfo* GuestInformConnector::selected_room;


void GuestMenuScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//

	//*** Material ***//

	//*** Mesh ***//

	///*** Game Object ***///

	auto mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		//mainCamera->AddComponent<CameraController>();
	}

	auto object = CreateEmpty();
	GuestInformConnector* informConnector = object->AddComponent<GuestInformConnector>();
	{
		GuestInformConnector::connector = informConnector;
	}

	auto background = CreateImage();
	{
		background->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("menuBackgroundMat");

		auto rectTransform = background->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 1, 1 };
	}
	informConnector->background = background;

	auto hostButtonPrefab = CreateImagePrefab();
	{
		auto rt = hostButtonPrefab->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0, 1);
		rt->setPosAndSize(50, -50, 100, 40);

		{
			auto textobject = hostButtonPrefab->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Guest Scene";
			text->fontSize = 20;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		hostButtonPrefab->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
		informConnector->buttonPrefab = hostButtonPrefab;
	}
}
