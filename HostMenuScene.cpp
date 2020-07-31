#include "pch.h"
#include "HostMenuScene.h"

HostInformConnector* HostInformConnector::connector{ nullptr };

void HostMenuScene::BuildObjects()
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
	HostInformConnector* informConnector = object->AddComponent<HostInformConnector>();
	{
		HostInformConnector::connector = informConnector;
	}

	auto background = CreateImage();
	{
		background->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("menuBackgroundMat");

		auto rectTransform = background->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 1, 1 };
		//
		//auto text = background->AddComponent<Text>();
		//text->text = L"두드리오\n\n\n";
		//text->font = L"메이플스토리";
		//text->fontSize = 120;
		//text->color = { 1, 1, 1, 1 };
		//text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		//text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

		auto newVillage = CreateImage();
		{
			auto rectTransform = newVillage->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = -250;
			rectTransform->posY = 20;
			rectTransform->width = 300;
			rectTransform->height = 400;

			newVillage->AddComponent<Button>()->AddEvent(
				[](void*) {		
					HostInformConnector::connector->goToNextPage();
				});
			{
				auto textobject = newVillage->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"New Village";
				text->font = L"메이플스토리";
				text->fontSize = 30;
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
			newVillage->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
			informConnector->newVillageButton = newVillage;
		}

		auto loadVillage = CreateImage();
		{
			auto rectTransform = loadVillage->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = 250;
			rectTransform->posY = 20;
			rectTransform->width = 300;
			rectTransform->height = 400;

			loadVillage->AddComponent<Button>()->AddEvent(
				[](void*) {
					Debug::Log("이게 되네;;\n");
				//	SceneManager::LoadScene("HostScene");
				});
			{
				auto textobject = loadVillage->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Load Village";
				text->font = L"메이플스토리";
				text->fontSize = 30;
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
			loadVillage->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
			informConnector->loadVillageButton = loadVillage;
		}
	}

}
