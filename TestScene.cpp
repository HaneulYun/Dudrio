#include "pch.h"
#include "TestScene.h"

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
	TerrainNodeData* terrainNodeData = new TerrainNodeData(&terrainData->terrainData);

	PathFinder::Instance()->SetTerrainData(&terrainData->terrainData, terrainNodeData);


	GameObject* mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		HostCameraController* controller = mainCamera->AddComponent<HostCameraController>();
		controller->terrainData = &terrainData->terrainData;
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

	GameObject* node = CreateEmptyPrefab();
	node->transform->Scale({ 2.f, 2.f, 2.f });
	node->AddComponent<MeshFilter>()->mesh = ASSET MESH("Cube");
	node->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("gray"));
	//PathFinder::Instance()->prefab = node;

//
//GameObject* manager = CreateEmpty();
//{
//	auto buildingBuilder = manager->AddComponent<BuildingBuilder>();
//	buildingBuilder->serializeBuildings();
//	buildingBuilder->terrain = terrainData;
//	buildingBuilder->cube = node;
//	buildingBuilder->terrainNodeData = terrainNodeData;
//	//BuildManager* buildManager = manager->AddComponent<BuildManager>();
//	//buildManager->terrain = terrain;
//	//buildManager->heightMap = &terrainData->terrainData;
//	//buildManager->terrainMesh = terrainData->terrainData.heightmapTexture;
//	//BuildManager::buildManager = buildManager;
//
//	AIManager::Instance = manager->AddComponent<AIManager>();
//}

	GameObject* pref = CreateEmptyPrefab();
	pref->transform->Scale({ 5.f, 5.f, 5.f });
	pref->AddComponent<MeshFilter>()->mesh = ASSET MESH("Cube");
	pref->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house01"));



	GameObject* landmark = CreateEmpty();
	landmark->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var02");
	landmark->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house02"));
	landmark->transform->position = Vector3(500, terrainData->terrainData.GetHeight(500, 500), 500);
	landmark->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
	Village* village = landmark->AddComponent<Village>();
	village->OnAutoDevelopment();



	GameObject* sim = CreateEmptyPrefab();
	//sim->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z), z);

	GameObject* model = sim->AddChild();
	model->transform->Rotate({ 1, 0, 0 }, -90);
	model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
	model->GetComponent<SkinnedMeshRenderer>()->materials.push_back(ASSET MATERIAL("PolyArt"));

	auto anim = model->AddComponent<Animator>();
	anim->controller = controller;
	anim->state = &controller->states["Idle"];
	anim->TimePos = 0;

	//auto simCompo = sim->AddComponent<Sim>();
	//simCompo->animator = anim;

	//AIManager::Instance->simPrefab = sim;
	


	auto object = CreateUI();
	{
		auto rt = object->GetComponent<RectTransform>();
		rt->width = CyanFW::Instance()->GetWidth();
		rt->height = CyanFW::Instance()->GetHeight();

		auto buildingBuilder = object->AddComponent<BuildingBuilder>();
		buildingBuilder->serializeBuildings();
		buildingBuilder->terrain = terrainData;
		buildingBuilder->terrainNodeData = terrainNodeData;
		//buildingBuilder->cube = node;
		AIManager::aiManager = object->AddComponent<AIManager>();
		//GameWorld::gameWorld = object->AddComponent<GameWorld>();
		//GameWorld::gameWorld->simPrefab = sim;

		auto buildingTypeSelector = object->AddComponent<BuildingTypeSelector>();
		buildingTypeSelector->builder = buildingBuilder;

		buildingTypeSelector->addBuildingType(BuildingBuilder::Landmark, L"랜드\n마크", -80, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::House, L"주거\n건물", -40, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::Theme, L"테마\n건물", 0, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::Landscape, L"조경", 40, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::Prop, L"소품", 80, 0);
	}


	for (int i = 0; i < 10; ++i)
	{
		int x = 450 + rand() % 100;
		int z = 450 + rand() % 100;

		GameObject* house = CreateEmpty();
		house->AddComponent<Building>();
		house->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var01");
		house->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house01"));
		house->AddComponent<BoxCollider>()->extents = (2.5, 2.5, 2.5);
		house->transform->position = Vector3(x, terrainData->terrainData.GetHeight(x, z), z);
		house->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);

		GameWorld::gameWorld->addSim(landmark, house);
	}
}