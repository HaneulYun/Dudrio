#include "pch.h"
#include "TerrainScene.h"

BuildManager* BuildManager::buildManager{ nullptr };
ButtonManager* ButtonManager::buttonManager{ nullptr };
GameLoader* GameLoader::gameLoader{ nullptr };

GameObject* TerrainScene::CreateTextButton()
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

GameObject* TerrainScene::CreateButtonList()
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
	button->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("grass");
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



void TerrainScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//

	{
		//ASSET AddTexture("none", L"Textures\\none.dds");
		ASSET AddTexture("ground", L"Textures\\grass.dds");
		ASSET AddTexture("grass", L"Textures\\grass01.dds");
		ASSET AddTexture("house01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_04_D.dds");
		ASSET AddTexture("house02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_09_D.dds");
		ASSET AddTexture("material_01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_01_D.dds");
		ASSET AddTexture("material_02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_02_D.dds");
		ASSET AddTexture("material_03", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_03_D.dds");
		//ASSET AddTexture("TreeLeafs", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeLeafs_D.dds");
		ASSET AddTexture("polyArtTex", L"Textures\\PolyArtTex.dds");
	}


	//*** Material ***//
	{
		//ASSET AddMaterial("none",			ASSET TEXTURE("none"));
		//ASSET AddMaterial("yellow",			ASSET TEXTURE("none"), -1, { 0.8f, 0.7f, 0.1f, 1.0f });
		ASSET AddMaterial("ground",			ASSET TEXTURE("ground"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("grass",			ASSET TEXTURE("grass"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
		ASSET AddMaterial("house01",		ASSET TEXTURE("house01"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("house02",		ASSET TEXTURE("house02"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_01",	ASSET TEXTURE("material_01"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_02",	ASSET TEXTURE("material_02"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_03",	ASSET TEXTURE("material_03"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("none", ASSET TEXTURE("none"));
		ASSET AddMaterial("PolyArt", ASSET TEXTURE("polyArtTex"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
	}

	//*** Mesh ***//
	{
		//ASSET AddMesh("Image", Mesh::CreateQuad());
		//ASSET AddMesh("Sphere", Mesh::CreateSphere());
		ASSET AddFbxForMesh("SM_Well", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well.FBX");
		ASSET AddFbxForMesh("SM_Well_Extra02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra02.FBX");
		ASSET AddFbxForMesh("SM_Well_Extra03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra03.FBX");
		ASSET AddFbxForMesh("SM_House_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
		ASSET AddFbxForMesh("SM_House_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02.FBX");
		ASSET AddFbxForMesh("SM_House_Var02_Extra", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02_Extra.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var01.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var02.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var03.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var04.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var05.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var06.FBX");
		ASSET AddFbxForMesh("SM_Cart_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var01.FBX");
		ASSET AddFbxForMesh("SM_Cart_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var02.FBX");
		ASSET AddFbxForMesh("SM_Barrel", "Assets\\AdvancedVillagePack\\Meshes\\SM_Barrel.FBX");
		ASSET AddFbxForMesh("SM_Bucket", "Assets\\AdvancedVillagePack\\Meshes\\SM_Bucket.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var01.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var02.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var03.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var04.FBX");
		ASSET AddFbxForMesh("SM_Cauldron", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cauldron.FBX");
		ASSET AddFbxForMesh("SM_Spike", "Assets\\AdvancedVillagePack\\Meshes\\SM_Spike.FBX");
		ASSET AddFbxForMesh("SM_Fish", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fish.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var01.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var02.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var03.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var04.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var05.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var06.FBX");
		ASSET AddFbxForMesh("SM_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Apple.FBX");
		ASSET AddFbxForMesh("SM_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Potato.FBX");
		ASSET AddFbxForMesh("SM_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tomato.FBX");
		ASSET AddFbxForMesh("SM_Watermellon", "Assets\\AdvancedVillagePack\\Meshes\\SM_Watermellon.FBX");
		ASSET AddFbxForMesh("SM_Sack_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Apple.FBX");
		ASSET AddFbxForMesh("SM_Sack_Flour", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Flour.FBX");
		ASSET AddFbxForMesh("SM_Sack_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Potato.FBX");
		ASSET AddFbxForMesh("SM_Sack_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Tomato.FBX");
		ASSET AddFbxForMesh("SM_Sack_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var01.FBX");
		ASSET AddFbxForMesh("SM_Sack_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var02.FBX");
	}

	ASSET AddFbxForAnimation("ApprenticeSK", "Models\\modelTest.fbx");

	CHeightMapImage* m_pHeightMapImage = new CHeightMapImage(L"Texture\\heightMap.raw", 257, 257, { 1.0f, 0.1f, 1.0f });
	CHeightMapGridMesh* gridMesh = new CHeightMapGridMesh(0, 0, 257, 257, { 1, 1, 1 }, { 1, 1, 0, 1 }, m_pHeightMapImage);
	
	//*** Animation ***//
	ASSET AddFbxForAnimation("Walk_BowAnim", "Models\\BowStance\\Walk_BowAnim.fbx");
	ASSET AddFbxForAnimation("WalkBack_BowAnim", "Models\\BowStance\\WalkBack_BowAnim.fbx");
	ASSET AddFbxForAnimation("WalkRight_BowAnim", "Models\\BowStance\\WalkRight_BowAnim.fbx");
	ASSET AddFbxForAnimation("WalkLeft_BowAnim", "Models\\BowStance\\WalkLeft_BowAnim.fbx");
	ASSET AddFbxForAnimation("Idle_BowAnim", "Models\\BowStance\\Idle_BowAnim.fbx");

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

	///*** Game Object ***///

	GameObject* mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<HostCameraController>();
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

	std::string name[9]{
		"Attack01_BowAnim",
		"Attack01Maintain_BowAnim",
		"Attack01RepeatFire_BowAnim",
		"Attack01Start_BowAnim",
		"Attack02Maintain_BowAnim",
		"Attack02RepeatFire_BowAnim",
		"Attack02Start_BowAnim",
		"DashBackward_BowAnim",
		"DashForward_BowAnim",
	};

	auto SimsPrefab = CreateEmptyPrefab();
	{
		auto model = SimsPrefab->AddChild();
		{
			model->GetComponent<Transform>()->Scale({ 0.02, 0.02, 0.02 });
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
		}
	}

	GameObject* grid = CreateEmpty();
	{
		grid->GetComponent<Transform>()->position -= {128, 10, 128};
		auto mesh = grid->AddComponent<MeshFilter>()->mesh = gridMesh;
		grid->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("ground"));
	}

	{
		GameObject* manager = CreateEmpty();
		BuildManager* buildManager = manager->AddComponent<BuildManager>();
		buildManager->terrain = grid;
		buildManager->heightMap = m_pHeightMapImage;
		buildManager->terrainMesh = gridMesh;
		BuildManager::buildManager = buildManager;
		ButtonManager* buttonManager = manager->AddComponent<ButtonManager>();
		ButtonManager::buttonManager = buttonManager;
		Builder* builder = manager->AddComponent<Builder>();
		Builder::builder = builder;
		GameLoader* gameload = manager->AddComponent<GameLoader>();
		GameLoader::gameLoader = gameload;
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

	GameObject* buttons_BuildingType[6];
	std::vector<GameObject*> butttons_BuildingList[6];

	for (int i = 0; i < 6; ++i)
	{
		buttons_BuildingType[i] = CreateTextButton();
		buttons_BuildingType[i]->GetComponent<RectTransform>()->posX = -80 + 40 * i;
	}
	buttons_BuildingType[0]->children.front()->GetComponent<Text>()->text = L"����\n��ũ";
	buttons_BuildingType[0]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::LandMark); });
	buttons_BuildingType[1]->children.front()->GetComponent<Text>()->text = L"�ְ�\n�ǹ�";
	buttons_BuildingType[1]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::House); });
	buttons_BuildingType[2]->children.front()->GetComponent<Text>()->text = L"�׸�\n�ǹ�";
	buttons_BuildingType[2]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Theme); });
	buttons_BuildingType[3]->children.front()->GetComponent<Text>()->text = L"����";
	buttons_BuildingType[3]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::landscape); });
	buttons_BuildingType[4]->children.front()->GetComponent<Text>()->text = L"��ǰ";
	buttons_BuildingType[4]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Decoration); });
	buttons_BuildingType[5]->children.front()->GetComponent<Text>()->text = L"����";
	buttons_BuildingType[5]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Delete); });

	for (int i = 0; i < 5; ++i)
	{
		butttons_BuildingList[i].push_back(CreateButtonList());
		ButtonManager::buttonManager->buttons_BuildingList[i].push_back(butttons_BuildingList[i][0]);
	}
	butttons_BuildingList[0][0]->children[0]->children[0]->GetComponent<Text>()->text = L"Well";
	butttons_BuildingList[0][0]->children[0]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Well_01); });

	butttons_BuildingList[1][0]->children[0]->children[0]->GetComponent<Text>()->text = L"House01";
	butttons_BuildingList[1][0]->children[0]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::House_01, ASSET MESH("SM_House_Var01"), ASSET MATERIAL("house01"), 0.02f, 5.5f); });
	butttons_BuildingList[1][0]->children[1]->children[0]->GetComponent<Text>()->text = L"House02";
	butttons_BuildingList[1][0]->children[1]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::House_02); });

	//SM_Tree_Var04
	butttons_BuildingList[3].push_back(CreateButtonList());
	butttons_BuildingList[3].push_back(CreateButtonList());
	ButtonManager::buttonManager->buttons_BuildingList[3].push_back(butttons_BuildingList[3][1]);
	ButtonManager::buttonManager->buttons_BuildingList[3].push_back(butttons_BuildingList[3][2]);
	butttons_BuildingList[3][0]->children[0]->children[0]->GetComponent<Text>()->text = L"Tree_01";
	//butttons_BuildingList[3][0]->children[0]->AddComponent<Button>()->AddEvent([](void*) {
	//	BuildManager::buildManager->SelectBuilding(BuildingType::Tree_01, ASSET MESH("SM_Tree_Var01"), ASSET MATERIAL("TreeLeafs"), 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[1]->children[0]->GetComponent<Text>()->text = L"Tree_02";
	//butttons_BuildingList[3][0]->children[1]->AddComponent<Button>()->AddEvent([](void*) {
	//	BuildManager::buildManager->SelectBuilding(BuildingType::Tree_02, ASSET MESH("SM_Tree_Var02"), ASSET MATERIAL("TreeLeafs"), 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[2]->children[0]->GetComponent<Text>()->text = L"Tree_03";
	//butttons_BuildingList[3][0]->children[2]->AddComponent<Button>()->AddEvent([](void*) {
	//	BuildManager::buildManager->SelectBuilding(BuildingType::Tree_03, ASSET MESH("SM_Tree_Var03"), ASSET MATERIAL("TreeLeafs"), 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[3]->children[0]->GetComponent<Text>()->text = L"Tree_04";
	//butttons_BuildingList[3][0]->children[3]->AddComponent<Button>()->AddEvent([](void*) {
	//	BuildManager::buildManager->SelectBuilding(BuildingType::Tree_04, ASSET MESH("SM_Tree_Var04"), ASSET MATERIAL("TreeLeafs"), 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[4]->children[0]->GetComponent<Text>()->text = L"Tree_05";
	//butttons_BuildingList[3][0]->children[4]->AddComponent<Button>()->AddEvent([](void*) {
	//	BuildManager::buildManager->SelectBuilding(BuildingType::Tree_05, ASSET MESH("SM_Tree_Var05"), ASSET MATERIAL("TreeLeafs"), 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[5]->children[0]->GetComponent<Text>()->text = L"Flower_01";
	butttons_BuildingList[3][0]->children[6]->children[0]->GetComponent<Text>()->text = L"Flower_02";
	butttons_BuildingList[3][0]->children[7]->children[0]->GetComponent<Text>()->text = L"Flower_03";
	butttons_BuildingList[3][0]->children[8]->children[0]->GetComponent<Text>()->text = L"Flower_04";
	butttons_BuildingList[3][0]->children[9]->children[0]->GetComponent<Text>()->text = L"Flower_05";

	butttons_BuildingList[3][1]->children[0]->children[0]->GetComponent<Text>()->text = L"Stone_Big_01";
	butttons_BuildingList[3][1]->children[1]->children[0]->GetComponent<Text>()->text = L"Stone_Big_02";
	butttons_BuildingList[3][1]->children[2]->children[0]->GetComponent<Text>()->text = L"Stone_Big_03";
	butttons_BuildingList[3][1]->children[3]->children[0]->GetComponent<Text>()->text = L"Stone_Big_04";
	butttons_BuildingList[3][1]->children[4]->children[0]->GetComponent<Text>()->text = L"Stone_Big_05";
	butttons_BuildingList[3][1]->children[5]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_01";
	butttons_BuildingList[3][1]->children[6]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_02";
	butttons_BuildingList[3][1]->children[7]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_03";
	butttons_BuildingList[3][1]->children[8]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_04";
	butttons_BuildingList[3][1]->children[9]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_05";

	butttons_BuildingList[3][2]->children[0]->children[0]->GetComponent<Text>()->text = L"Mushroom_01";
	butttons_BuildingList[3][2]->children[0]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_01, ASSET MESH("SM_Mushroom_Var01"), ASSET MATERIAL("material_02"), 0.02f, 0.5f); });
	butttons_BuildingList[3][2]->children[1]->children[0]->GetComponent<Text>()->text = L"Mushroom_02";
	butttons_BuildingList[3][2]->children[1]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_02, ASSET MESH("SM_Mushroom_Var02"), ASSET MATERIAL("material_02"), 0.02f, 0.5f); });
	butttons_BuildingList[3][2]->children[2]->children[0]->GetComponent<Text>()->text = L"Mushroom_03";
	butttons_BuildingList[3][2]->children[2]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_03, ASSET MESH("SM_Mushroom_Var03"), ASSET MATERIAL("material_02"), 0.02f, 0.5f); });
	butttons_BuildingList[3][2]->children[3]->children[0]->GetComponent<Text>()->text = L"Mushroom_04";
	butttons_BuildingList[3][2]->children[3]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_04, ASSET MESH("SM_Mushroom_Var04"), ASSET MATERIAL("material_02"), 0.02f, 0.5f); });
	butttons_BuildingList[3][2]->children[4]->children[0]->GetComponent<Text>()->text = L"Mushroom_05";
	butttons_BuildingList[3][2]->children[4]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_05, ASSET MESH("SM_Mushroom_Var05"), ASSET MATERIAL("material_02"), 0.02f, 0.4f); });
	butttons_BuildingList[3][2]->children[5]->children[0]->GetComponent<Text>()->text = L"Mushroom_06";
	butttons_BuildingList[3][2]->children[5]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Mushroom_06, ASSET MESH("SM_Mushroom_Var06"), ASSET MATERIAL("material_02"), 0.02f, 0.4f); });
	//butttons_BuildingList[4][1]->children[6]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_02";
	//butttons_BuildingList[4][1]->children[7]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_03";
	//butttons_BuildingList[4][1]->children[8]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_04";
	//butttons_BuildingList[4][1]->children[9]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_05";

	butttons_BuildingList[4].push_back(CreateButtonList());
	butttons_BuildingList[4].push_back(CreateButtonList());
	ButtonManager::buttonManager->buttons_BuildingList[4].push_back(butttons_BuildingList[4][1]);
	ButtonManager::buttonManager->buttons_BuildingList[4].push_back(butttons_BuildingList[4][2]);
	butttons_BuildingList[4][0]->children[0]->children[0]->GetComponent<Text>()->text = L"Fence_01";
	butttons_BuildingList[4][0]->children[0]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_01); });
	butttons_BuildingList[4][0]->children[1]->children[0]->GetComponent<Text>()->text = L"Fence_02";
	butttons_BuildingList[4][0]->children[1]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_02); });
	butttons_BuildingList[4][0]->children[2]->children[0]->GetComponent<Text>()->text = L"Fence_03";
	butttons_BuildingList[4][0]->children[2]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_03); });
	butttons_BuildingList[4][0]->children[3]->children[0]->GetComponent<Text>()->text = L"Fence_04";
	butttons_BuildingList[4][0]->children[3]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Fence_04); });
	butttons_BuildingList[4][0]->children[4]->children[0]->GetComponent<Text>()->text = L"Bucket";
	butttons_BuildingList[4][0]->children[4]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Bucket, ASSET MESH("SM_Bucket"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });
	butttons_BuildingList[4][0]->children[5]->children[0]->GetComponent<Text>()->text = L"Barrel";
	butttons_BuildingList[4][0]->children[5]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Barrel, ASSET MESH("SM_Barrel"), ASSET MATERIAL("material_01"), 0.02f, 1.5f); });
	butttons_BuildingList[4][0]->children[6]->children[0]->GetComponent<Text>()->text = L"Pitchfork";
	butttons_BuildingList[4][0]->children[7]->children[0]->GetComponent<Text>()->text = L"Axe";
	butttons_BuildingList[4][0]->children[8]->children[0]->GetComponent<Text>()->text = L"Ladder";
	butttons_BuildingList[4][0]->children[9]->children[0]->GetComponent<Text>()->text = L"Spike";
	butttons_BuildingList[4][0]->children[9]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Spike); });

	butttons_BuildingList[4][1]->children[0]->children[0]->GetComponent<Text>()->text = L"Cart_01";
	butttons_BuildingList[4][1]->children[0]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Cart_01); });
	butttons_BuildingList[4][1]->children[1]->children[0]->GetComponent<Text>()->text = L"Cart_02";
	butttons_BuildingList[4][1]->children[1]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Cart_02); });

	butttons_BuildingList[4][1]->children[2]->children[0]->GetComponent<Text>()->text = L"Cauldron";
	butttons_BuildingList[4][1]->children[2]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Cauldron, ASSET MESH("SM_Cauldron"), ASSET MATERIAL("material_02"), 0.02f, 1.0f); });
	//butttons_BuildingList[4][1]->children[3]->children[0]->GetComponent<Text>()->text = L"Torch_02";
	butttons_BuildingList[4][1]->children[4]->children[0]->GetComponent<Text>()->text = L"Pot_01";
	butttons_BuildingList[4][1]->children[4]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Pot_01, ASSET MESH("SM_Pot_Var01"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });
	butttons_BuildingList[4][1]->children[5]->children[0]->GetComponent<Text>()->text = L"Pot_02";
	butttons_BuildingList[4][1]->children[5]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Pot_02, ASSET MESH("SM_Pot_Var02"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });
	butttons_BuildingList[4][1]->children[6]->children[0]->GetComponent<Text>()->text = L"Pot_03";
	butttons_BuildingList[4][1]->children[6]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Pot_03, ASSET MESH("SM_Pot_Var03"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });
	butttons_BuildingList[4][1]->children[7]->children[0]->GetComponent<Text>()->text = L"Pot_04";
	butttons_BuildingList[4][1]->children[7]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Pot_04, ASSET MESH("SM_Pot_Var04"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });
	butttons_BuildingList[4][1]->children[8]->children[0]->GetComponent<Text>()->text = L"Pot_05";
	butttons_BuildingList[4][1]->children[8]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Pot_05, ASSET MESH("SM_Pot_Var05"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });
	butttons_BuildingList[4][1]->children[9]->children[0]->GetComponent<Text>()->text = L"Pot_06";
	butttons_BuildingList[4][1]->children[9]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Pot_06, ASSET MESH("SM_Pot_Var06"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });

	butttons_BuildingList[4][2]->children[0]->children[0]->GetComponent<Text>()->text = L"Apple";
	butttons_BuildingList[4][2]->children[0]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Apple, ASSET MESH("SM_Apple"), ASSET MATERIAL("material_02"), 0.02f, 0.2f); });
	butttons_BuildingList[4][2]->children[1]->children[0]->GetComponent<Text>()->text = L"Potato";
	butttons_BuildingList[4][2]->children[1]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Potato, ASSET MESH("SM_Potato"), ASSET MATERIAL("material_02"), 0.02f, 0.2f); });
	butttons_BuildingList[4][2]->children[2]->children[0]->GetComponent<Text>()->text = L"Tomato";
	butttons_BuildingList[4][2]->children[2]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Tomato, ASSET MESH("SM_Tomato"), ASSET MATERIAL("material_02"), 0.02f, 0.3f); });
	butttons_BuildingList[4][2]->children[3]->children[0]->GetComponent<Text>()->text = L"Watermellon";
	butttons_BuildingList[4][2]->children[3]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Watermellon, ASSET MESH("SM_Watermellon"), ASSET MATERIAL("material_02"), 0.02f, 0.6f); });
	butttons_BuildingList[4][2]->children[4]->children[0]->GetComponent<Text>()->text = L"Sack_Apple";
	butttons_BuildingList[4][2]->children[4]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Apple, ASSET MESH("SM_Sack_Apple"), ASSET MATERIAL("material_02"), 0.02f, 1.0f); });
	butttons_BuildingList[4][2]->children[5]->children[0]->GetComponent<Text>()->text = L"Sack_Flour";
	butttons_BuildingList[4][2]->children[5]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Flour, ASSET MESH("SM_Sack_Flour"), ASSET MATERIAL("material_02"), 0.02f, 0.9f); });
	butttons_BuildingList[4][2]->children[6]->children[0]->GetComponent<Text>()->text = L"Sack_Potato";
	butttons_BuildingList[4][2]->children[6]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Potato, ASSET MESH("SM_Sack_Potato"), ASSET MATERIAL("material_02"), 0.02f, 1.0f); });
	butttons_BuildingList[4][2]->children[7]->children[0]->GetComponent<Text>()->text = L"Sack_Tomato";
	butttons_BuildingList[4][2]->children[7]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Sack_Tomato, ASSET MESH("SM_Sack_Tomato"), ASSET MATERIAL("material_02"), 0.02f, 1.0f); });

	butttons_BuildingList[4][2]->children[8]->children[0]->GetComponent<Text>()->text = L"Sack_01";
	butttons_BuildingList[4][2]->children[8]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Sack_01, ASSET MESH("SM_Sack_Var01"), ASSET MATERIAL("material_02"), 0.02f, 1.0f); });
	butttons_BuildingList[4][2]->children[9]->children[0]->GetComponent<Text>()->text = L"Sack_02";
	butttons_BuildingList[4][2]->children[9]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Sack_02, ASSET MESH("SM_Sack_Var02"), ASSET MATERIAL("material_02"), 0.02f, 0.8f); });

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