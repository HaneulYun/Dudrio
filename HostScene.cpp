#include "pch.h"
#include "HostScene.h"

GameLoader* GameLoader::gameLoader{ nullptr };
BuildingBuilder* BuildingBuilder::buildingBuilder{ nullptr };
GameWorld* GameWorld::gameWorld;
AIManager* AIManager::aiManager;

void HostScene::BuildObjects()
{
	/***Object Tag***/
	spatialPartitioningManager.tagData.AddTag("Building");
	spatialPartitioningManager.tagData.AddTag("Preview");
	//spatialPartitioningManager.tagData.SetTagCollision(TAG_BUILDING, TAG_PREVIEW, true);

	///*** Asset ***///
	LoadTextureAsset();
	LoadMaterialAsset();
	LoadMeshAsset();

	LoadPrefab();

	ASSET AddMesh("Cube", Mesh::CreateCube());


	//*** AnimatorController ***//
	AnimatorController* controller = new AnimatorController();
	{
		controller->AddParameterFloat("VelocityX");
		controller->AddParameterFloat("VelocityZ");

		controller->AddState("Idle", ASSET animationClips["Idle_BowAnim"].get());
		controller->AddState("Walk", ASSET animationClips["Walk_BowAnim"].get());
		controller->AddState("WalkBack", ASSET animationClips["WalkBack_BowAnim"].get());
		controller->AddState("WalkRight", ASSET animationClips["WalkRight_BowAnim"].get());
		controller->AddState("WalkLeft", ASSET animationClips["WalkLeft_BowAnim"].get());

		controller->AddTransition("Idle", "Walk", TransitionCondition::CreateFloat("VelocityZ", Greater, 0.3));
		controller->AddTransition("Idle", "WalkBack", TransitionCondition::CreateFloat("VelocityZ", Less, -0.3));
		controller->AddTransition("Walk", "Idle", TransitionCondition::CreateFloat("VelocityZ", Less, 0.3));
		controller->AddTransition("WalkBack", "Idle", TransitionCondition::CreateFloat("VelocityZ", Greater, -0.3));

		controller->AddTransition("Idle", "WalkLeft", TransitionCondition::CreateFloat("VelocityX", Greater, 0.3));
		controller->AddTransition("Idle", "WalkRight", TransitionCondition::CreateFloat("VelocityX", Less, -0.3));
		controller->AddTransition("WalkLeft", "Idle", TransitionCondition::CreateFloat("VelocityX", Less, 0.3));
		controller->AddTransition("WalkRight", "Idle", TransitionCondition::CreateFloat("VelocityX", Greater, -0.3));
	}

	//*** DummySimController ***//
	AnimatorController* simController = new AnimatorController();
	{
		simController->AddParameterFloat("Walking");

		simController->AddState("Idle", ASSET animationClips["Idle_BowAnim"].get());
		simController->AddState("Walk", ASSET animationClips["Walk_BowAnim"].get());

		simController->AddTransition("Idle", "Walk", TransitionCondition::CreateFloat("Walking", Greater, 1));
		simController->AddTransition("Walk", "Idle", TransitionCondition::CreateFloat("Walking", Less, 1));
	}

	{
		GameObject* fps = CreateUI();
		auto rectTransform = fps->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 1, 1 };

		Text* text = fps->AddComponent<Text>();
		text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		text->color = { 1,1,1,1 };
		fps->AddComponent<FPS>();
	}

	auto simsPrefab = CreateEmptyPrefab();
	{
		auto model = simsPrefab->AddChild();
		{
			model->GetComponent<Transform>()->Rotate({ 1, 0, 0 }, -90);
			auto mesh = model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
			auto renderer = model->GetComponent<SkinnedMeshRenderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(ASSET MATERIAL("PolyArt"));
		}
		auto behavior = simsPrefab->AddComponent<CharacterMovingBehavior>();
		auto anim = simsPrefab->AddComponent<Animator>();
		anim->controller = controller;
		anim->state = &controller->states["Idle"];
		anim->TimePos = 0;
		behavior->anim = anim;
	}


	float TerrainSize = HostInformConnector::connector->terrainSize;
	float frequency = HostInformConnector::connector->frequency;
	int octaves = HostInformConnector::connector->octaves;
	int seed = HostInformConnector::connector->seed;

	TerrainGenerator* terrainGenerator = new TerrainGenerator(TerrainSize, TerrainSize);
	string fileName = terrainGenerator->createHeightMap(frequency, octaves, seed, (char*)"square");
	delete terrainGenerator;

	GameObject* terrain = CreateEmpty();
	auto terrainData = terrain->AddComponent<Terrain>();
	simsPrefab->GetComponent<CharacterMovingBehavior>()->heightmap = &terrainData->terrainData;
	{
		wstring name;
		name.assign(fileName.begin(), fileName.end());
		terrainData->terrainData.AlphamapTextureName = name.c_str();
		terrainData->terrainData.heightmapHeight = TerrainSize;
		terrainData->terrainData.heightmapWidth = TerrainSize;
		terrainData->terrainData.size = { TerrainSize, 255, TerrainSize };
		terrainData->Set();

		terrain->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("ground"));
	}

	TerrainNodeData* terrainNodeData = new TerrainNodeData(&terrainData->terrainData);
	PathFinder::Instance()->SetTerrainData(&terrainData->terrainData, terrainNodeData);


	///*** Game Object ***///
	GameObject* mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		HostCameraController* controller = mainCamera->AddComponent<HostCameraController>();
		controller->terrainData = &terrainData->terrainData;
	}

	{
		auto skySphere = CreateEmpty();
		skySphere->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		skySphere->AddComponent<MeshFilter>()->mesh = ASSET MESH("Sphere");
		skySphere->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));
		skySphere->layer = (int)RenderLayer::Sky;
	}

	auto directionalLight = CreateEmpty();
	{
		//directionalLight->transform->Rotate({ 1, 0, 0 }, 60);
		auto light = directionalLight->AddComponent<Light>();
		light->Strength = { 0.9f, 0.9f, 0.9f };
		light->shadowType = Light::Shadows;

		environment.sunSources = light;
	}

	//GameObject* manager = CreateEmpty();
	//{
	//	BuildManager* buildManager = manager->AddComponent<BuildManager>();
	//	buildManager->terrain = terrain;
	//	buildManager->heightMap = &terrainData->terrainData;
	//	buildManager->terrainMesh = terrainData->terrainData.heightmapTexture;
	//	BuildManager::buildManager = buildManager;
	//
	//	Builder* builder = manager->AddComponent<Builder>();
	//	Builder::builder = builder;
	//
	//	GameLoader* gameload = manager->AddComponent<GameLoader>();
	//	GameLoader::gameLoader = gameload;
	//}

	/*particle*/
	//const int particleNum = 10;
	//for (int i = 0; i < particleNum; ++i)
	//{
	//	auto particleSystemObjectSmoke = CreateEmpty();
	//	ParticleSystem* particleSystem = particleSystemObjectSmoke->AddComponent<ParticleSystem>();
	//	particleSystem->Set();
	//	particleSystem->enabled = false;
	//	particleSystemObjectSmoke->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("smokeMat"));
	//	particleSystemObjectSmoke->layer = (int)RenderLayer::Particle;
	//	BuildManager::buildManager->particles.push_back(particleSystemObjectSmoke->AddComponent<ParticleManager>());
	//}

	auto sim = CreateEmptyPrefab();
	{
		auto model = sim->AddChild();
		{
			model->transform->Rotate({ 1, 0, 0 }, -90);
			model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
			model->GetComponent<SkinnedMeshRenderer>()->materials.push_back(ASSET MATERIAL("PolyArt"));
		}
		sim->AddComponent<Sim>();

		auto anim = sim->AddComponent<Animator>();
		anim->controller = simController;
		anim->state = &simController->states["Idle"];
		anim->TimePos = 0;
	}

	GameObject* node = CreateEmpty();
	node->transform->Scale(Vector3(0.5, 0.5, 0.5));
	node->AddComponent<MeshFilter>()->mesh = ASSET MESH("Cube");
	node->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));


	auto object = CreateUI();
	{
		auto rt = object->GetComponent<RectTransform>();
		rt->width = CyanFW::Instance()->GetWidth();
		rt->height = CyanFW::Instance()->GetHeight();
		auto buildingBuilder = object->AddComponent<BuildingBuilder>();
		buildingBuilder->serializeBuildings();
		buildingBuilder->terrain = terrainData;
		buildingBuilder->terrainNodeData = terrainNodeData;
		buildingBuilder->cube = node;

		GameWorld* gameWorld = object->AddComponent<GameWorld>();
		gameWorld->simPrefab = sim;
		gameWorld->sun = directionalLight;
		object->AddComponent<AIManager>();

		auto buildingTypeSelector = object->AddComponent<BuildingTypeSelector>();
		buildingTypeSelector->builder = buildingBuilder;
		buildingTypeSelector->addBuildingType(BuildingBuilder::Landmark, L"랜드\n마크", -80, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::House, L"주거\n건물", -40, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::Theme, L"테마\n건물", 0, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::Landscape, L"조경", 40, 0);
		buildingTypeSelector->addBuildingType(BuildingBuilder::Prop, L"소품", 80, 0);
	}

	auto network = CreateEmpty();
	HostNetwork* hostNetwork = network->AddComponent<HostNetwork>();
	{
		HostNetwork::network = hostNetwork;
		hostNetwork->simsPrefab = simsPrefab;
		hostNetwork->terrainSize = TerrainSize;
		hostNetwork->frequency = frequency;
		hostNetwork->octaves = octaves;
		hostNetwork->seed = seed;
		strcpy_s(hostNetwork->name, HostInformConnector::connector->name);
	}

	auto ServerButton = CreateImage();
	{
		auto rt = ServerButton->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0, 1);
		rt->setPosAndSize(1110, -10, 80, 30);
	
		ServerButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				HostNetwork::network->PressButton();
			});
		{
			auto textobject = ServerButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };
	
			Text* text = textobject->AddComponent<Text>();
			text->text = L"Open";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			hostNetwork->connectButtonText = text;
		}
	}

	auto LoadButton = CreateImage();
	{
		auto rt = LoadButton->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0, 1);
		rt->setPosAndSize(1110, -50, 80, 30);

		LoadButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				GameLoader::gameLoader->Load();
			});
		{
			auto textobject = LoadButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Load";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
	}

	auto SaveButton = CreateImage();
	{
		auto rt = SaveButton->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0, 1);
		rt->setPosAndSize(1110, -90, 80, 30);

		SaveButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				GameLoader::gameLoader->Save();
			});
		{
			auto textobject = SaveButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Save";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
	}

	auto menuSceneButton = CreateImage();
	{
		auto rt = menuSceneButton->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0, 1);
		rt->setPosAndSize(10, -10, 150, 30);

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
		}
	}
}