#include "pch.h"
#include "TestScene.h"

AIManager* AIManager::Instance;

void TestScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	ASSET AddTexture("ground", L"Textures\\grass.dds");
	ASSET AddTexture("grass", L"Texture\\grass.dds");
	ASSET AddTexture("house01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_04_D.dds");
	ASSET AddTexture("house02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_09_D.dds");
	ASSET AddTexture("polyArtTex", L"Textures\\PolyArtTex.dds");

	//*** Material ***//
	ASSET AddMaterial("ground", ASSET TEXTURE("ground"), nullptr, { 0.48f, 0.64f, 0.2f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f, Matrix4x4::MatrixScaling(200, 200, 200));
	ASSET AddMaterial("grass", ASSET TEXTURE("grass"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
	ASSET AddMaterial("house01", ASSET TEXTURE("house01"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
	ASSET AddMaterial("house02", ASSET TEXTURE("house02"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
	ASSET AddMaterial("PolyArt", ASSET TEXTURE("polyArtTex"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);

	//*** Mesh 
	ASSET AddFbxForMesh("SM_House_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
	ASSET AddFbxForMesh("SM_House_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02.FBX");
	ASSET AddMesh("Cube", Mesh::CreateCube());

	ASSET AddFbxForAnimation("ApprenticeSK", "Models\\modelTest.fbx");

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

		AIManager::Instance = manager->AddComponent<AIManager>();

	}

	GameObject* pref = CreateEmptyPrefab();
	pref->transform->Scale({ 5.f, 5.f, 5.f });
	pref->AddComponent<MeshFilter>()->mesh = ASSET MESH("Cube");
	pref->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house01"));

	GameObject* node = CreateEmptyPrefab();
	node->transform->Scale({2.f, 2.f, 2.f });
	node->AddComponent<MeshFilter>()->mesh = ASSET MESH("Cube");
	node->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("gray"));
	PathFinder::Instance()->prefab = node;


	GameObject* landmark = CreateEmpty();
	Village* village = landmark->AddComponent<Village>();
	village->OnAutoDevelopment();
	landmark->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var02");
	landmark->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house02"));
	landmark->transform->position = Vector3(500, terrainData->terrainData.GetHeight(500, 500), 500);
	landmark->transform->Scale({ 0.01f, 0.01f, 0.01f });
	landmark->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);


	for (int i = 0; i < 1; ++i)
	{
		int x = rand() % 60 + 470;
		int z = rand() % 60 + 470;

		GameObject* house = CreateEmpty();
		house->AddComponent<Building>();
		house->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var01");
		house->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house01"));
		house->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z), z);
		house->transform->Scale({ 0.01f, 0.01f, 0.01f });
		house->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);

		GameObject* sim = CreateEmpty();
		sim->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z - 5), z - 5);
		GameObject* model = sim->AddChild();
		model->AddComponent<MeshFilter>()->mesh = ASSET MESH("ApprenticeSK");
		model->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("PolyArt"));
		model->transform->Scale({ 0.01, 0.01, 0.01 });
		model->transform->Rotate({ 1, 0, 0 }, -90);
		Sim* simCompo = sim->AddComponent<Sim>();
		simCompo->home = house;
		AIManager::Instance->AddSim(simCompo);

		village->sims.push_back(sim);
		simCompo->prefab = pref;
	}
}