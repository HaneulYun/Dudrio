#include "pch.h"
#include "TestScene.h"

void TestScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	ASSET AddTexture("ground", L"Textures\\grass.dds");
	ASSET AddTexture("grass", L"Texture\\grass.dds");

	//*** Material ***//
	ASSET AddMaterial("ground", ASSET TEXTURE("ground"), -1, { 0.48f, 0.64f, 0.2f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f, Matrix4x4::MatrixScaling(200, 200, 200));
	ASSET AddMaterial("grass", ASSET TEXTURE("grass"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);

	//*** Mesh 

	///*** Game Object ***///
	auto menuSceneButton = CreateImage();
	{
		auto rectTransform = menuSceneButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 1 };
		rectTransform->anchorMax = { 0, 1 };
		rectTransform->pivot = { 0, 1 };
		rectTransform->posX = 10;
		rectTransform->posY = -10;
		rectTransform->width = 150;
		rectTransform->height = 30;

		menuSceneButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				SceneManager::LoadScene("MenuScene");
			});
		{
			auto textobject = menuSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Menu Scene";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	{
		GameObject* fps = CreateUI();
		auto rectTransform = fps->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 1, 1 };

		Text* text = fps->AddComponent<Text>();
		text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		text->color = { 1,1,1,1 };
		textObjects.push_back(fps);
		fps->AddComponent<FPS>();
	}


	/*************************************************************/
	float TerrainSize = 1024;
	float frequency = 4;
	int octaves = 1;
	int seed = 100;

	TerrainGenerator* terrainGenerator = new TerrainGenerator(TerrainSize, TerrainSize);
	string fileName = terrainGenerator->createHeightMap(frequency, octaves, seed, (char*)"square");
	//delete terrainGenerator;
	/*************************************************************/


	GameObject* terrain = CreateEmpty();
	auto terrainData = terrain->AddComponent<Terrain>();
	{
		{
			wstring name;
			name.assign(fileName.begin(), fileName.end());
			terrainData->terrainData.AlphamapTextureName = name.c_str();
			terrainData->terrainData.heightmapHeight = TerrainSize;
			terrainData->terrainData.heightmapWidth = TerrainSize;

			terrainData->terrainData.size = { TerrainSize, 255, TerrainSize };

			terrainData->Set();
		}
		terrain->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("ground"));
	}

	GameObject* mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		HostCameraController* controller = mainCamera->AddComponent<HostCameraController>();
		controller->heightmap = &terrainData->terrainData;
	}

	{
		auto ritem = CreateEmpty();
		ritem->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		auto mesh = ritem->AddComponent<MeshFilter>()->mesh = ASSET MESH("Sphere");
		auto renderer = ritem->AddComponent<Renderer>();
		for (auto& sm : mesh->DrawArgs)
			renderer->materials.push_back(ASSET MATERIAL("none"));
		ritem->layer = (int)RenderLayer::Sky;
	}

}