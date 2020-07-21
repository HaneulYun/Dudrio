#include "pch.h"
#include "HostScene.h"

BuildManager* BuildManager::buildManager{ nullptr };
ButtonManager* ButtonManager::buttonManager{ nullptr };
GameLoader* GameLoader::gameLoader{ nullptr };

GameObject* HostScene::CreateTextButton()
{
	GameObject* button = CreateImage();
	{
		auto rectTransform = button->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = 0;
		rectTransform->posY = 0;
		rectTransform->width = 40;
		rectTransform->height = 40;

		{
			auto textobject = button->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"X";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}
	return button;
}

GameObject* HostScene::CreateButtonList()
{
	GameObject* button = CreateImage();
	{
		auto rectTransform = button->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = 40;
		rectTransform->posY = 40;
		rectTransform->width = CyanFW::Instance()->GetWidth() - 80;
		rectTransform->height = 60;
	}
	button->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("house01");
	button->SetActive(false);

	for (int k = 0; k < 10; ++k)
	{
		GameObject* button_building = CreateTextButton();
		button_building->SetActive(false);
		auto rectTransform = button_building->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.05f + 0.1f * k, 0.1 };
		rectTransform->anchorMax = { 0.1f * k + 0.09f, 0.9 };
		button->AddChild(button_building);
	}
	return button;
}



void HostScene::BuildObjects()
{
	///*** Asset ***///
	LoadTextureAsset();
	LoadMaterialAsset();
	LoadMeshAsset();

	LoadPrefab();

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

	float TerrainSize = 1024;
	float frequency = 5;
	int octaves = 3;
	int seed = 1024;

	TerrainGenerator* terrainGenerator = new TerrainGenerator(TerrainSize, TerrainSize);
	string fileName = terrainGenerator->createHeightMap(frequency, octaves, seed, (char*)"square");
	delete terrainGenerator;

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

	///*** Game Object ***///

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

	auto SimsPrefab = CreateEmptyPrefab();
	{
		auto model = SimsPrefab->AddChild();
		{
			model->GetComponent<Transform>()->Scale({ 0.01, 0.01, 0.01 });
			model->GetComponent<Transform>()->Rotate({ 1, 0, 0 }, -90);
			auto mesh = model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
			auto renderer = model->GetComponent<SkinnedMeshRenderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(ASSET MATERIAL("PolyArt"));
	
			auto anim = model->AddComponent<Animator>();
			anim->controller = controller;
			anim->state = &controller->states["Idle"];
			anim->TimePos = 0;
	
			SimsPrefab->AddComponent<CharacterMovingBehavior>()->anim = anim;
			SimsPrefab->GetComponent<CharacterMovingBehavior>()->heightmap = &terrainData->terrainData;
		}
	}

	GameObject* manager = CreateEmpty();
	{
		BuildManager* buildManager = manager->AddComponent<BuildManager>();
		buildManager->terrain = terrain;
		buildManager->heightMap = &terrainData->terrainData;
		buildManager->terrainMesh = terrainData->terrainData.heightmapTexture;
		BuildManager::buildManager = buildManager;

		ButtonManager* buttonManager = manager->AddComponent<ButtonManager>();
		ButtonManager::buttonManager = buttonManager;

		Builder* builder = manager->AddComponent<Builder>();
		Builder::builder = builder;

		GameLoader* gameload = manager->AddComponent<GameLoader>();
		GameLoader::gameLoader = gameload;

	}
	ButtonManager* buttonManager = ButtonManager::buttonManager = manager->AddComponent<ButtonManager>();

	/*particle*/
	const int particleNum = 10;
	for (int i = 0; i < particleNum; ++i)
	{
		auto particleSystemObjectSmoke = CreateEmpty();
		ParticleSystem* particleSystem = particleSystemObjectSmoke->AddComponent<ParticleSystem>();
		particleSystem->Set();
		particleSystem->enabled = false;
		particleSystemObjectSmoke->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("smokeMat"));
		particleSystemObjectSmoke->layer = (int)RenderLayer::Particle;
		BuildManager::buildManager->particles.push_back(particleSystemObjectSmoke->AddComponent<ParticleManager>());
	}


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

	//auto object = CreateUI();
	//{
	//	auto buildingBuilder = object->AddComponent<BuildingBuilder>();
	//
	//	auto buildingTypeSelector = object->AddComponent<BuildingTypeSelector>();
	//	buildingTypeSelector->builder = buildingBuilder;
	//
	//	auto buildingType = buildingTypeSelector->addBuildingType(L"랜드\n마크", -80, 0);
	//	buildingTypeSelector->addBuildingType(L"주거\n건물", -40, 0);
	//	buildingTypeSelector->addBuildingType(L"테마\n건물", 0, 0);
	//	buildingTypeSelector->addBuildingType(L"조경", 40, 0);
	//	buildingTypeSelector->addBuildingType(L"소품", 80, 0);
	//}

	GameObject* buttons_BuildingType[6];

	for (int i = 0; i < 6; ++i)
	{
		buttons_BuildingType[i] = CreateTextButton();
		buttons_BuildingType[i]->GetComponent<RectTransform>()->posX = -80 + 40 * i;
	}
	buttons_BuildingType[0]->children.front()->GetComponent<Text>()->text = L"랜드\n마크";
	buttons_BuildingType[0]->AddComponent<Button>()->AddEvent( [](void* ptr) { ButtonManager::buttonManager->SelectButton(ButtonType::LandMark); });
	buttons_BuildingType[1]->children.front()->GetComponent<Text>()->text = L"주거\n건물";
	buttons_BuildingType[1]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::House); });
	buttons_BuildingType[2]->children.front()->GetComponent<Text>()->text = L"테마\n건물";
	buttons_BuildingType[2]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Theme); });
	buttons_BuildingType[3]->children.front()->GetComponent<Text>()->text = L"조경";
	buttons_BuildingType[3]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::landscape); });
	buttons_BuildingType[4]->children.front()->GetComponent<Text>()->text = L"소품";
	buttons_BuildingType[4]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Decoration); });
	buttons_BuildingType[5]->children.front()->GetComponent<Text>()->text = L"삭제";
	buttons_BuildingType[5]->AddComponent<Button>()->AddEvent([](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Delete); 
	BuildManager::buildManager->deleteButtonState = !BuildManager::buildManager->deleteButtonState;
		if (BuildManager::buildManager->prefab) BuildManager::buildManager->DeletePrefab();
		});

	/*LandMark*/
	for (auto& LandMarkAssetInfo :
		{
			ButtonAssetItem{L"Well",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Well_01); }}
		})
		buttonManager->buttonAssetItem[0].push_back(LandMarkAssetInfo);

	/*House*/
	for (auto& HouseAssetInfo :
		{
			ButtonAssetItem{L"House01",		[](void*) {BuildManager::buildManager->SelectBuilding(BuildingType::House_01,		ASSET MESH("SM_House_Var01"), ASSET MATERIAL("house01"), 0.01f, 2.75f); }},
			ButtonAssetItem{L"House02",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::House_02); }}
		})
		buttonManager->buttonAssetItem[1].push_back(HouseAssetInfo);

	/*Theme*/
	
	/*landscape*/
	for (auto& landscapeAssetInfo :
		{
			ButtonAssetItem{L"Tree_01",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Tree_01, ASSET MESH("SM_Tree_Var01"), ASSET MATERIAL("TreeLeafs"), 0.01f, 5.0f); }},
			ButtonAssetItem{L"Tree_02",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Tree_02, ASSET MESH("SM_Tree_Var02"), ASSET MATERIAL("TreeLeafs"), 0.01f, 5.0f); }},
			ButtonAssetItem{L"Tree_03",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Tree_03, ASSET MESH("SM_Tree_Var03"), ASSET MATERIAL("TreeLeafs"), 0.01f, 5.0f); }},
			ButtonAssetItem{L"Tree_04",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Tree_04, ASSET MESH("SM_Tree_Var04"), ASSET MATERIAL("TreeLeafs"), 0.01f, 5.0f); }},
			ButtonAssetItem{L"Tree_05",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Tree_05, ASSET MESH("SM_Tree_Var05"), ASSET MATERIAL("TreeLeafs"), 0.01f, 5.0f); }},
			//ButtonAssetItem{L"Flower_01"},
			//ButtonAssetItem{L"Flower_02"},
			//ButtonAssetItem{L"Flower_03"},
			//ButtonAssetItem{L"Flower_04"},
			//ButtonAssetItem{L"Flower_05"},
			//ButtonAssetItem{L"Stone_Big_01"},
			//ButtonAssetItem{L"Stone_Big_02"},
			//ButtonAssetItem{L"Stone_Big_03"},
			//ButtonAssetItem{L"Stone_Big_04"},
			//ButtonAssetItem{L"Stone_Big_05"},			
			//ButtonAssetItem{L"Stone_Medium_01"},
			//ButtonAssetItem{L"Stone_Medium_02"},
			//ButtonAssetItem{L"Stone_Medium_03"},
			//ButtonAssetItem{L"Stone_Medium_04"},
			//ButtonAssetItem{L"Stone_Medium_05"},
			ButtonAssetItem{L"Mushroom_01",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_01,	ASSET MESH("SM_Mushroom_Var01"), ASSET MATERIAL("material_02"), 0.01f, 0.2f); }},
			ButtonAssetItem{L"Mushroom_02",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_02,	ASSET MESH("SM_Mushroom_Var02"), ASSET MATERIAL("material_02"), 0.01f, 0.2f); }},
			ButtonAssetItem{L"Mushroom_03",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_03,	ASSET MESH("SM_Mushroom_Var03"), ASSET MATERIAL("material_02"), 0.01f, 0.2f); }},
			ButtonAssetItem{L"Mushroom_04",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_04,	ASSET MESH("SM_Mushroom_Var04"), ASSET MATERIAL("material_02"), 0.01f, 0.2f); }},
			ButtonAssetItem{L"Mushroom_05",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_05,	ASSET MESH("SM_Mushroom_Var05"), ASSET MATERIAL("material_02"), 0.01f, 0.15f); }},
			ButtonAssetItem{L"Mushroom_06",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_06,	ASSET MESH("SM_Mushroom_Var06"), ASSET MATERIAL("material_02"), 0.01f, 0.15f); }}
		})
		buttonManager->buttonAssetItem[3].push_back(landscapeAssetInfo);

	for (auto& btnAssetInfo :
		{
			ButtonAssetItem{L"Fence_01",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_01); }},
			ButtonAssetItem{L"Fence_02",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_02); }},
			ButtonAssetItem{L"Fence_03",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_03); }},
			ButtonAssetItem{L"Fence_04",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_04); }},
			ButtonAssetItem{L"Bucket",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Bucket,		ASSET MESH("SM_Bucket"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},
			ButtonAssetItem{L"Barrel",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Barrel,		ASSET MESH("SM_Barrel"), ASSET MATERIAL("material_01"), 0.01f, 0.75f); }},
			//ButtonAssetItem{L"Pitchfork"},
			//ButtonAssetItem{L"Axe"},
			//ButtonAssetItem{L"Ladder"},
			ButtonAssetItem{L"Spike",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Spike); }},

			ButtonAssetItem{L"Cart_01",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Cart_01); }},
			ButtonAssetItem{L"Cart_02",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Cart_02); }},
			ButtonAssetItem{L"Cauldron",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Cauldron,		ASSET MESH("SM_Cauldron"), ASSET MATERIAL("material_02"), 0.01f, 0.5f); }},
			//ButtonAssetItem{L"Torch_02"},
			ButtonAssetItem{L"Pot_01",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Pot_01,		ASSET MESH("SM_Pot_Var01"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},
			ButtonAssetItem{L"Pot_02",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Pot_02,		ASSET MESH("SM_Pot_Var02"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},
			ButtonAssetItem{L"Pot_03",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Pot_03,		ASSET MESH("SM_Pot_Var03"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},
			ButtonAssetItem{L"Pot_04",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Pot_04,		ASSET MESH("SM_Pot_Var04"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},
			ButtonAssetItem{L"Pot_05",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Pot_05,		ASSET MESH("SM_Pot_Var05"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},
			ButtonAssetItem{L"Pot_06",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Pot_06,		ASSET MESH("SM_Pot_Var06"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},

			ButtonAssetItem{L"Apple",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Apple,			ASSET MESH("SM_Apple"), ASSET MATERIAL("material_02"), 0.01f, 0.1f); }},
			ButtonAssetItem{L"Potato",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Potato,		ASSET MESH("SM_Potato"), ASSET MATERIAL("material_02"), 0.01f, 0.1f); }},
			ButtonAssetItem{L"Tomato",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Tomato,		ASSET MESH("SM_Tomato"), ASSET MATERIAL("material_02"), 0.01f, 0.15f); }},
			ButtonAssetItem{L"Watermellon",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Watermellon,	ASSET MESH("SM_Watermellon"), ASSET MATERIAL("material_02"), 0.01f, 0.3f); }},
			ButtonAssetItem{L"Sack_Apple",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Apple,	ASSET MESH("SM_Sack_Apple"), ASSET MATERIAL("material_02"), 0.01f, 0.5f); }},
			ButtonAssetItem{L"Sack_Flour",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Flour,	ASSET MESH("SM_Sack_Flour"), ASSET MATERIAL("material_02"), 0.01f, 0.5f); }},
			ButtonAssetItem{L"Sack_Potato",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Potato,	ASSET MESH("SM_Sack_Potato"), ASSET MATERIAL("material_02"), 0.01f, 0.5f); }},
			ButtonAssetItem{L"Sack_Tomato",	[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Tomato,	ASSET MESH("SM_Sack_Tomato"), ASSET MATERIAL("material_02"), 0.01f, 0.5f); }},
			ButtonAssetItem{L"Sack_01",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Sack_01,		ASSET MESH("SM_Sack_Var01"), ASSET MATERIAL("material_02"), 0.01f, 0.5f); }},
			ButtonAssetItem{L"Sack_02",		[](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Sack_02,		ASSET MESH("SM_Sack_Var02"), ASSET MATERIAL("material_02"), 0.01f, 0.4f); }},
		})
		buttonManager->buttonAssetItem[4].push_back(btnAssetInfo);


	GameObject* button_previouspage = CreateImage();
	{
		auto rectTransform = button_previouspage->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = 10;
		rectTransform->posY = 60;
		rectTransform->width = 20;
		rectTransform->height = 20;

		button_previouspage->AddComponent<Button>()->AddEvent([](void*) { ButtonManager::buttonManager->PreviousPage(); });
		button_previouspage->SetActive(false);
		ButtonManager::buttonManager->buttons_page[0] = button_previouspage;
	}
	GameObject* button_nextpage = CreateImage();
	{
		auto rectTransform = button_nextpage->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = CyanFW::Instance()->GetWidth() - 30;
		rectTransform->posY = 60;
		rectTransform->width = 20;
		rectTransform->height = 20;

		button_nextpage->AddComponent<Button>()->AddEvent([](void*) { ButtonManager::buttonManager->NextPage(); });
		button_nextpage->SetActive(false);
		ButtonManager::buttonManager->buttons_page[1] = button_nextpage;
	}

	auto network = CreateEmpty();
	{
		HostNetwork* hn = network->AddComponent<HostNetwork>();
		hn->simsPrefab = SimsPrefab;
		HostNetwork::network = hn;
		hn->terrainSize = TerrainSize;
		hn->frequency = frequency;
		hn->octaves = octaves;
		hn->seed = seed;
	}

	auto ServerButton = CreateImage();
	{
		auto rectTransform = ServerButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 1 };
		rectTransform->anchorMax = { 0, 1 };
		rectTransform->pivot = { 0, 1 };
		rectTransform->posX = 1110;
		rectTransform->posY = -10;
		rectTransform->width = 80;
		rectTransform->height = 30;
	
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
			textObjects.push_back(textobject);
		}
	}

	auto LoadButton = CreateImage();
	{
		auto rectTransform = LoadButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 1 };
		rectTransform->anchorMax = { 0, 1 };
		rectTransform->pivot = { 0, 1 };
		rectTransform->posX = 1110;
		rectTransform->posY = -50;
		rectTransform->width = 80;
		rectTransform->height = 30;

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
			textObjects.push_back(textobject);
		}
	}

	auto SaveButton = CreateImage();
	{
		auto rectTransform = SaveButton->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 1 };
		rectTransform->anchorMax = { 0, 1 };
		rectTransform->pivot = { 0, 1 };
		rectTransform->posX = 1110;
		rectTransform->posY = -90;
		rectTransform->width = 80;
		rectTransform->height = 30;

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
			textObjects.push_back(textobject);
		}
	}
}