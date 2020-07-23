#include "pch.h"
#include "TestScene.h"

AIManager* AIManager::Instance; 

void TestScene::BuildObjects()
{
	LoadTextureAsset();
	LoadMaterialAsset();
	LoadMeshAsset();

	LoadPrefab();

	//*** AnimatorController ***//
	AnimatorController* controller = new AnimatorController();
	{
		controller->AddParameterFloat("Walking");

		controller->AddState("Idle", ASSET animationClips["Idle_BowAnim"].get());
		controller->AddState("Walk", ASSET animationClips["Walk_BowAnim"].get());

		controller->AddTransition("Idle", "Walk", TransitionCondition::CreateFloat("Walking", Greater, 1));
		controller->AddTransition("Walk", "Idle", TransitionCondition::CreateFloat("Walking", Less, 1));
	}


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
		auto buildingBuilder = manager->AddComponent<BuildingBuilder>();
		buildingBuilder->serializeBuildings();
		buildingBuilder->terrain = terrainData;
		//BuildManager* buildManager = manager->AddComponent<BuildManager>();
		//buildManager->terrain = terrain;
		//buildManager->heightMap = &terrainData->terrainData;
		//buildManager->terrainMesh = terrainData->terrainData.heightmapTexture;
		//BuildManager::buildManager = buildManager;

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
	landmark->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);


	for (int i = 0; i < 2; ++i)
	{
		int x = 500;
		int z = 500;

		GameObject* house = CreateEmpty();
		house->AddComponent<Building>();
		house->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var01");
		house->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house01"));
		house->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z), z);
		house->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);

		GameObject* sim = CreateEmpty();
		sim->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z), z);

		GameObject* model = sim->AddChild();
		model->transform->Rotate({ 1, 0, 0 }, -90);
		model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
		model->GetComponent<SkinnedMeshRenderer>()->materials.push_back(ASSET MATERIAL("PolyArt"));

		auto anim = model->AddComponent<Animator>();
		anim->controller = controller;
		anim->state = &controller->states["Idle"];
		anim->TimePos = 0;

		Sim* simCompo = sim->AddComponent<Sim>();
		simCompo->home = house;
		simCompo->animator = anim;

		AIManager::Instance->AddSim(simCompo);

		village->sims.push_back(sim);
	}
}