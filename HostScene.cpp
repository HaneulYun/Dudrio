#include "pch.h"
#include "HostScene.h"

BuildingBuilder* BuildingBuilder::buildingBuilder{ nullptr };
GameLoader* GameLoader::gameLoader{ nullptr };
HostGameWorld* HostGameWorld::gameWorld;
AIManager* AIManager::aiManager;

void HostScene::BuildObjects()
{
	/***Object Tag***/
	spatialPartitioningManager.tagData.AddTag("Building");
	spatialPartitioningManager.tagData.AddTag("Preview");
	//spatialPartitioningManager.tagData.SetTagCollision(TAG_BUILDING, TAG_PREVIEW, true);

	////*** AudioClip ***//
	ASSET AddAudioClip("bgm_ingame", "Sounds\\bgm_ingame.mp3");

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

	//{
	//	GameObject* fps = CreateUI();
	//	auto rectTransform = fps->GetComponent<RectTransform>();
	//	rectTransform->anchorMin = { 0, 0 };
	//	rectTransform->anchorMax = { 1, 1 };
	//
	//	Text* text = fps->AddComponent<Text>();
	//	text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	//	text->color = { 1,1,1,1 };
	//	fps->AddComponent<FPS>();
	//}

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

	float TerrainSize = 1000;
	int frequency, octaves, seed;
	int file_pointer;
	float gametime = 0.0f;
	int gameday = 1;
	wstring host_name;
	if (!HostInformConnector::connector->load) {
		TerrainSize = HostInformConnector::connector->terrainSize;
		frequency = HostInformConnector::connector->frequency;
		octaves = HostInformConnector::connector->octaves;
		seed = HostInformConnector::connector->seed;
		host_name = HostInformConnector::connector->name;

		GameLoader::gameLoader->initFile(host_name, frequency, octaves, seed);
	}
	else {
		file_pointer = GameLoader::gameLoader->Load(host_name, frequency, octaves, seed, gametime, gameday);
	}

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
		mainCamera->AddComponent<AudioListener>();
		auto controller = mainCamera->AddComponent<HostCameraController>();
		controller->terrainData = &terrainData->terrainData;
	}

	auto soundBox = CreateEmpty();
	{
		auto audioSource = soundBox->AddComponent<AudioSource>();
		audioSource->clip = ASSET AUDIO_CLIP("bgm_ingame");
		audioSource->loop = true;
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

	auto sea = CreateEmpty();
	{
		sea->transform->position = { 500, 1, 500 };
		sea->transform->Scale({ 10000, 1, 10000 });
		sea->AddComponent<MeshFilter>()->mesh = ASSET MESH("Plane");
		sea->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("sea"));
	}
	auto bottom = CreateEmpty();
	{
		bottom->transform->Scale({ 10000, 1, 10000 });
		bottom->transform->Rotate({ 0, 0, 1 }, 180);
		bottom->transform->position = { 500, 1, 500 };
		bottom->AddComponent<MeshFilter>()->mesh = ASSET MESH("Plane");
		bottom->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));
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


	auto ui_bar = Scene::scene->CreateImage();
	{
		auto rt = ui_bar->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0, 0);
		rt->setPosAndSize(0, 0, CyanFW::Instance()->GetWidth(), 25);

		ui_bar->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_bar");
	}

	GameUI* gameUI;
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
		BuildingBuilder::buildingBuilder = buildingBuilder;

		HostGameWorld* gameWorld = object->AddComponent<HostGameWorld>();
		gameWorld->simPrefab = sim;
		gameWorld->sun = directionalLight;
		if (HostInformConnector::connector->load) {
			gameWorld->gameTime = gametime;
			gameWorld->day = gameday;
		}
		object->AddComponent<AIManager>();
		auto gameLoader = object->AddComponent<GameLoader>();
		HostGameWorld::gameWorld = gameWorld;

		auto buildingTypeSelector = object->AddComponent<BuildingTypeSelector>();
		buildingTypeSelector->builder = buildingBuilder;
		buildingTypeSelector->addBuildingType(BuildingBuilder::Landmark, -140, 0, ASSET MATERIAL("icon_landmark"));
		buildingTypeSelector->addBuildingType(BuildingBuilder::House, -100, 0, ASSET MATERIAL("icon_house"));
		buildingTypeSelector->addBuildingType(BuildingBuilder::Theme, -60, 0, ASSET MATERIAL("icon_theme"));
		buildingTypeSelector->addBuildingType(BuildingBuilder::Landscape, -20, 0, ASSET MATERIAL("icon_landscape"));
		buildingTypeSelector->addBuildingType(BuildingBuilder::Lighting, 20, 0, ASSET MATERIAL("icon_lighting"));
		buildingTypeSelector->addBuildingType(BuildingBuilder::Fence, 60, 0, ASSET MATERIAL("icon_fence"));
		buildingTypeSelector->addBuildingType(BuildingBuilder::Prop, 100, 0, ASSET MATERIAL("icon_prop"));
		buildingTypeSelector->addDeleteButton(140, 0, ASSET MATERIAL("icon_delete"));

		gameUI = object->AddComponent<GameUI>();
		gameUI->gameUIs.push_back(ui_bar);
		gameWorld->gameUI = gameUI;
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
		hostNetwork->gameUI = gameUI;
		wcscpy_s(hostNetwork->name, host_name.c_str());
		HostNetwork::network = hostNetwork;
	}

	gameUI->gameUIs.push_back(Scene::scene->CreateImage());
	{
		auto rt = gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0.5, 0.5);
		rt->setPosAndSize(0, 0, 220, 190);

		auto serverOpenButton = gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->AddChildUI(Scene::scene->CreateImagePrefab());
		serverOpenButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_bar_dark");
		{
			auto rt = serverOpenButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0.5);
			rt->setPosAndSize(0, -60, 200, 50);

			Text* text = serverOpenButton->AddComponent<Text>();
			text->fontSize = 35;
			text->text = L"오픈하기";
			text->color = { 0.9140625f, 0.796875f, 0.37890625f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			serverOpenButton->AddComponent<Button>()->AddEvent([](void* ptr)
				{
					HostNetwork::network->PressButton();
				});
			HostNetwork::network->connectButtonText = text;
		}

		auto gameLoadButton = gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->AddChildUI(Scene::scene->CreateImagePrefab());
		gameLoadButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_bar_dark");
		{
			auto rt = gameLoadButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0.5);
			rt->setPosAndSize(0, 0, 200, 50);

			Text* text = gameLoadButton->AddComponent<Text>();
			text->fontSize = 35;
			text->text = L"저장하기";
			text->color = { 0.9140625f, 0.796875f, 0.37890625f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			gameLoadButton->AddComponent<Button>()->AddEvent([](void* ptr)
				{
					GameLoader::gameLoader->Save(HostNetwork::network->name, HostNetwork::network->frequency, HostNetwork::network->octaves, HostNetwork::network->seed);
					GameLoader::gameLoader->SaveTime(HostGameWorld::gameWorld->gameTime, HostGameWorld::gameWorld->day);
				});
		}

		auto gameExitButton = gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->AddChildUI(Scene::scene->CreateImagePrefab());
		gameExitButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_bar_dark");
		{
			auto rt = gameExitButton->GetComponent<RectTransform>();
			rt->setAnchorAndPivot(0.5, 0.5);
			rt->setPosAndSize(0, 60, 200, 50);

			Text* text = gameExitButton->AddComponent<Text>();
			text->fontSize = 35;
			text->text = L"게임종료";
			text->color = { 0.9140625f, 0.796875f, 0.37890625f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			gameExitButton->AddComponent<Button>()->AddEvent([](void* ptr)
				{
					GameLoader::gameLoader->SaveTime(HostGameWorld::gameWorld->gameTime, HostGameWorld::gameWorld->day);
					PostQuitMessage(0);
				});
		}

		gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_bar");
	}
	gameUI->gameUIs[GameUI::GameUICategory::MenuUI]->SetActive(false);

	gameUI->gameUIs.push_back(Scene::scene->CreateUI());
	{
		auto rt = gameUI->gameUIs[GameUI::GameUICategory::TimeX1]->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(1, 0);
		rt->setPosAndSize(-170, 0, 15, 25);

		gameUI->gameUIs[GameUI::GameUICategory::TimeX1]->AddComponent<Button>()->AddEvent([](void* ptr)
			{
				if (!HostNetwork::network->isConnect)
					HostGameWorld::gameWorld->timeSpeed = HostGameWorld::TimeSpeed::X1;
			});
	}

	gameUI->gameUIs.push_back(Scene::scene->CreateUI());
	{
		auto rt = gameUI->gameUIs[GameUI::GameUICategory::TimeX2]->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(1, 0);
		rt->setPosAndSize(-150, 0, 15, 25);

		gameUI->gameUIs[GameUI::GameUICategory::TimeX2]->AddComponent<Button>()->AddEvent([](void* ptr)
			{
				if (!HostNetwork::network->isConnect)
					HostGameWorld::gameWorld->timeSpeed = HostGameWorld::TimeSpeed::X2;
			});
	}

	gameUI->gameUIs.push_back(Scene::scene->CreateUI());
	{
		auto rt = gameUI->gameUIs[GameUI::GameUICategory::TimeX4]->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(1, 0);
		rt->setPosAndSize(-125, 0, 20, 25);

		gameUI->gameUIs[GameUI::GameUICategory::TimeX4]->AddComponent<Button>()->AddEvent([](void* ptr)
			{
				if (!HostNetwork::network->isConnect)
					HostGameWorld::gameWorld->timeSpeed = HostGameWorld::TimeSpeed::X4;
			});
	}

	gameUI->gameUIs.push_back(Scene::scene->CreateImage());
	{
		auto rt = gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(1, 1);
		rt->setPosAndSize(-10, -10, 170, 220);

		{
			auto textObj = gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->AddChildUI();
			auto trt = textObj->GetComponent<RectTransform>();
			trt->setAnchorAndPivot(0, 0);
			trt->setPosAndSize(111, 100, 63, 55);
			Text* text = textObj->AddComponent<Text>();
			text->fontSize = 16;
			text->font = L"배달의민족 도현";
			//text->text = L"150m\n\n10명";
			text->color = { 0.9140625f, 0.796875f, 0.37890625f, 1.0f };
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}

		{
			auto onButton = gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->AddChildUI();
			auto onrt = onButton->GetComponent<RectTransform>();
			onrt->setAnchorAndPivot(0, 0);
			onrt->setPosAndSize(15, 8, 65, 25);

			auto offButton = gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->AddChildUI();
			auto offrt = offButton->GetComponent<RectTransform>();
			offrt->setAnchorAndPivot(0, 0);
			offrt->setPosAndSize(90, 8, 65, 25);


			onButton->AddComponent<Button>()->AddEvent([](void* ptr)
				{
					//Debug::Log("자동건설 on\n");
					BuildingBuilder::buildingBuilder->curLandmark->GetComponent<Village>()->OnAutoDevelopment();
					if (HostNetwork::network->mainConnect)
						HostNetwork::network->send_landmark_change_packet(
							BuildingBuilder::buildingBuilder->curLandmark->transform->position.x,
							BuildingBuilder::buildingBuilder->curLandmark->transform->position.z, true);
				});

			offButton->AddComponent<Button>()->AddEvent([](void* ptr)
				{
					//Debug::Log("자동건설 off\n");
					BuildingBuilder::buildingBuilder->curLandmark->GetComponent<Village>()->OffAutoDevelopment();
					if (HostNetwork::network->mainConnect)
						HostNetwork::network->send_landmark_change_packet(
							BuildingBuilder::buildingBuilder->curLandmark->transform->position.x,
							BuildingBuilder::buildingBuilder->curLandmark->transform->position.z, false);
				});

		}
		gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("ui_landmark_Inform");
	}
	gameUI->gameUIs[GameUI::GameUICategory::LandMarkUI]->SetActive(false);

	if (HostInformConnector::connector->load)
		GameLoader::gameLoader->LoadBuildings(file_pointer);
}