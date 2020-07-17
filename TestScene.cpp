#include "pch.h"
#include "TestScene.h"

void TestScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	ASSET AddTexture("ground", L"Textures\\grass.dds");
	ASSET AddTexture("grass", L"Texture\\grass.dds");
	ASSET AddTexture("house01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_04_D.dds");

	//*** Material ***//
	ASSET AddMaterial("ground", ASSET TEXTURE("ground"), -1, { 0.48f, 0.64f, 0.2f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f, Matrix4x4::MatrixScaling(200, 200, 200));
	ASSET AddMaterial("grass", ASSET TEXTURE("grass"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
	ASSET AddMaterial("house01", ASSET TEXTURE("house01"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);

	//*** Mesh 
	ASSET AddFbxForMesh("SM_House_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
	ASSET AddMesh("Cube", Mesh::CreateCube());

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
	delete terrainGenerator;
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
	PathFinder::Instance()->SetTerrainData(&terrainData->terrainData);


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

	GameObject* manager = CreateEmpty();
	{
		BuildManager* buildManager = manager->AddComponent<BuildManager>();
		buildManager->terrain = terrain;
		buildManager->heightMap = &terrainData->terrainData;
		buildManager->terrainMesh = terrainData->terrainData.heightmapTexture;
		BuildManager::buildManager = buildManager;

		manager->AddComponent<AIManager>();
	}

	GameObject* pref = CreateEmptyPrefab();
	pref->transform->Scale({ 5.f, 5.f, 5.f });
	pref->AddComponent<MeshFilter>()->mesh = ASSET MESH("Cube");
	pref->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("gray"));
	PathFinder::Instance()->prefab = pref;

	for (int i = 0; i < 10; ++i)
	{
		int x = rand() % 300 + 400;
		int z = rand() % 300 + 400;

		GameObject* house = CreateEmpty();
		house->AddComponent<Building>();
		house->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var01");
		house->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house01"));
		house->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z), z);
		house->transform->Scale({ 0.01f, 0.01f, 0.01f });
		house->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);

		GameObject* sim = CreateEmpty();
		sim->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z - 5), z - 5);
		sim->AddComponent<MeshFilter>()->mesh = ASSET MESH("Cube");
		sim->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));
		sim->transform->Scale({ 20.f, 20.f, 20.f });
		Sim* simCompo = sim->AddComponent<Sim>();
		simCompo->home = house;
		SimManager::Instance()->AddSim(simCompo);
	}
}