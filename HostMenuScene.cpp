#include "pch.h"
#include "HostMenuScene.h"

HostInformConnector* HostInformConnector::connector{ nullptr };
wchar_t HostInformConnector::name[MAX_ID_LEN + 1];
float HostInformConnector::terrainSize = 1000.0f;
int HostInformConnector::frequency;
int HostInformConnector::octaves;
int HostInformConnector::seed;
bool HostInformConnector::load;

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
		auto gameLoader = object->AddComponent<GameLoader>();
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
			rectTransform->setAnchorAndPivot(0.5, 0.5);
			rectTransform->setPosAndSize(-250, 0, 420, 550);

			newVillage->AddComponent<Button>()->AddEvent(
				[](void*) {		
					HostInformConnector::connector->load = false;
					HostInformConnector::connector->goToNextPage();
				});

			newVillage->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("start_world");
			informConnector->newVillageButton = newVillage;
		}

		auto loadVillage = CreateImage();
		{
			auto rectTransform = loadVillage->GetComponent<RectTransform>();
			rectTransform->setAnchorAndPivot(0.5, 0.5);
			rectTransform->setPosAndSize(250, 0, 420, 550);

			loadVillage->AddComponent<Button>()->AddEvent(
				[](void*) {
					HostInformConnector::connector->load = true;
					SceneManager::LoadScene("HostScene");
				});

			loadVillage->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("load_world");
			informConnector->loadVillageButton = loadVillage;
		}
	}

}
