#include "pch.h"
#include "SampleScene.h"

Builder* Builder::builder{ nullptr };

void SampleScene::BuildObjects()
{
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
		ASSET AddMaterial("ground", ASSET TEXTURE("ground"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("grass", ASSET TEXTURE("grass"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
		ASSET AddMaterial("house01", ASSET TEXTURE("house01"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("house02", ASSET TEXTURE("house02"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_01", ASSET TEXTURE("material_01"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_02", ASSET TEXTURE("material_02"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_03", ASSET TEXTURE("material_03"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("PolyArt", ASSET TEXTURE("polyArtTex"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
	}

	//*** Mesh ***//
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

	auto mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<HostCameraController>();	// GuestCameraController·Î ¹Ù²ã¾ßÇÔ
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

	auto grid = CreateEmpty();
	{
		grid->GetComponent<Transform>()->position -= {128, 10, 128};
		auto mesh = grid->AddComponent<MeshFilter>()->mesh = gridMesh;
		grid->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("ground"));
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

	{
		GameObject* build = CreateEmpty();
		Builder* bd = build->AddComponent<Builder>();
		Builder::builder = bd;
	}

	auto network = CreateEmpty();
	{
		network->AddComponent<GuestNetwork>()->simsPrefab = SimsPrefab;
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

}