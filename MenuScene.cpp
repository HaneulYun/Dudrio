#include "pch.h"
#include "MenuScene.h"

void MenuScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	AddTexture(0, "none", L"Textures\\none.dds");

	//*** Material ***//
	AddMaterial(0, "none", 0);

	//*** Mesh ***//
	geometries["Image"] = Mesh::CreateQuad();
	geometries["Sphere"] = Mesh::CreateSphere();

	///*** Game Object ***///

	auto mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<CameraController>();
	}

	auto skyBox = CreateEmpty();
	{
		skyBox->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		skyBox->AddComponent<Renderer>()->materials.push_back(1);
		auto mesh = skyBox->AddComponent<MeshFilter>()->mesh = geometries["Sphere"].get();
		renderObjectsLayer[(int)RenderLayer::Sky][mesh].gameObjects.push_back(skyBox);
	}

	auto sampleSceneButton = CreateImage();
	{
		auto rectTransform = sampleSceneButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 1, 0.5 };
		rectTransform->anchorMax = { 1, 0.5 };
		rectTransform->pivot = { 1, 0.5 };
		rectTransform->posX = -10;
		rectTransform->posY = 20;
		rectTransform->width = 150;
		rectTransform->height = 30;

		sampleSceneButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				Debug::Log("�̰� �ǳ�;;\n");
				SceneManager::LoadScene("MainScene");
			});
		{
			auto textobject = sampleSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Sample Scene";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto materialSceneButton = CreateImage();
	{
		auto rectTransform = materialSceneButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 1, 0.5 };
		rectTransform->anchorMax = { 1, 0.5 };
		rectTransform->pivot = { 1, 0.5 };
		rectTransform->posX = -10;
		rectTransform->posY = -20;
		rectTransform->width = 150;
		rectTransform->height = 30;

		materialSceneButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				SceneManager::LoadScene("MaterialScene");
			});
		{
			auto textobject = materialSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Material Scene";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto terrainSceneButton = CreateImage();
	{
		auto rectTransform = terrainSceneButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 1, 0.5 };
		rectTransform->anchorMax = { 1, 0.5 };
		rectTransform->pivot = { 1, 0.5 };
		rectTransform->posX = -10;
		rectTransform->posY = -60;
		rectTransform->width = 150;
		rectTransform->height = 30;

		terrainSceneButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				SceneManager::LoadScene("TerrainScene");
			});
		{
			auto textobject = terrainSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Terrain Scene";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}


}