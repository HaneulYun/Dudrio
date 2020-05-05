#include "pch.h"
#include "MenuScene.h"

void MenuScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	ASSET AddTexture("none", L"Textures\\none.dds");

	//*** Material ***//
	ASSET AddMaterial("none", ASSET TEXTURE("none"));
	//ASSET AddMaterial("gray", ASSET TEXTURE("none"), -1, { 0.5, 0.5, 0.5, 0.5 });

	//*** Mesh ***//
	ASSET AddMesh("Image", Mesh::CreateQuad());
	ASSET AddMesh("Sphere", Mesh::CreateSphere());

	///*** Game Object ***///

	auto mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<CameraController>();
	}

	auto skyBox = CreateEmpty();
	{
		skyBox->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		skyBox->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));
		auto mesh = skyBox->AddComponent<MeshFilter>()->mesh = ASSET MESH("Sphere");
		skyBox->layer = (int)RenderLayer::Sky;
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
				Debug::Log("이게 되네;;\n");
				//SceneManager::LoadScene("MainScene");
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
		sampleSceneButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
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
				//SceneManager::LoadScene("MaterialScene");
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
		materialSceneButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
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

	auto animationSceneButton = CreateImage();
	{
		auto rectTransform = animationSceneButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 1, 0.5 };
		rectTransform->anchorMax = { 1, 0.5 };
		rectTransform->pivot = { 1, 0.5 };
		rectTransform->posX = -10;
		rectTransform->posY = -100;
		rectTransform->width = 150;
		rectTransform->height = 30;

		animationSceneButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				SceneManager::LoadScene("AnimationScene");
			});
		{
			auto textobject = animationSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Animation Scene";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto particleSceneButton = CreateImage();
	{
		auto rectTransform = particleSceneButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 1, 0.5 };
		rectTransform->anchorMax = { 1, 0.5 };
		rectTransform->pivot = { 1, 0.5 };
		rectTransform->posX = -10;
		rectTransform->posY = -140;
		rectTransform->width = 150;
		rectTransform->height = 30;

		particleSceneButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				SceneManager::LoadScene("ParticleScene");
			});
		{
			auto textobject = particleSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Particle Scene";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}
}
