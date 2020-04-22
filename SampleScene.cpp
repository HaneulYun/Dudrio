#include "pch.h"
#include "SampleScene.h"

Scene* Scene::scene{ nullptr };

void SampleScene::BuildObjects()
{
	///*** Asset ***///

	//*** Texture ***//
	{
		AddTexture(0, "none", L"Textures\\none.dds");
		AddTexture(1, "polyArtTex", L"Textures\\PolyArtTex.dds");
		AddTexture(2, "bricksTex", L"Textures\\bricks.dds");
		AddTexture(3, "stoneTex", L"Textures\\stone.dds");
		AddTexture(4, "tileTex", L"Textures\\tile.dds");
	}

	//*** Material ***//
	{
		for (int i = 1; i < 20; ++i)
		{
			auto material = std::make_unique<Material>();
			material->Name = "material_" + std::to_string(i);
			material->MatCBIndex = i;
			material->DiffuseSrvHeapIndex = 0;
			material->NormalSrvHeapIndex = 0;
			material->DiffuseAlbedo = RANDOM_COLOR;
			material->FresnelR0 = { 0.01f, 0.01f, 0.01f };
			material->Roughness = 0.0f;
			materials[material->Name] = std::move(material);
		}

		{
			auto material = std::make_unique<Material>();
			material->Name = "PolyArt";
			material->MatCBIndex = 0;
			material->DiffuseSrvHeapIndex = 1;
			material->NormalSrvHeapIndex = 0;
			material->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
			material->FresnelR0 = { 0.01f, 0.01f, 0.01f };
			material->Roughness = 0.0f;
			materials[material->Name] = std::move(material);

			auto material_bricks0 = std::make_unique<Material>();
			material_bricks0->Name = "bricksMat";
			material_bricks0->MatCBIndex = 1;
			material_bricks0->DiffuseSrvHeapIndex = 2;
			material_bricks0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			material_bricks0->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
			material_bricks0->Roughness = 0.1f;
			materials[material_bricks0->Name] = std::move(material_bricks0);

			auto material_stone0 = std::make_unique<Material>();
			material_stone0->Name = "stoneMat";
			material_stone0->MatCBIndex = 2;
			material_stone0->DiffuseSrvHeapIndex = 3;
			material_stone0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			material_stone0->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
			material_stone0->Roughness = 0.3f;
			materials[material_stone0->Name] = std::move(material_stone0);

			auto material_tile0 = std::make_unique<Material>();
			material_tile0->Name = "tile0";
			material_tile0->MatCBIndex = 3;
			material_tile0->DiffuseSrvHeapIndex = 4;
			material_tile0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			material_tile0->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
			material_tile0->Roughness = 0.2f;
			XMStoreFloat4x4(&material_tile0->MatTransform, XMMatrixScaling(8.0f, 8.0f, 1.0f));
			materials[material_tile0->Name] = std::move(material_tile0);
		}
	}

	//*** Mesh ***//
	geometries["Cube"] = Mesh::CreateCube();
	geometries["Plane"] = Mesh::CreatePlane();
	geometries["Sphere"] = Mesh::CreateSphere();
	geometries["Cylinder"] = Mesh::CreateCylinder();
	AddFbxForAnimation("ApprenticeSK", "Models\\modelTest.fbx");

	CHeightMapImage* m_pHeightMapImage = new CHeightMapImage(L"Texture\\heightMap.raw", 257, 257, { 1.0f, 0.1f, 1.0f });
	CHeightMapGridMesh* gridMesh = new CHeightMapGridMesh(0, 0, 257, 257, { 1, 1, 1 }, { 1, 1, 0, 1 }, m_pHeightMapImage);

	//*** Animation ***//
	{
		AddFbxForAnimation("Attack01_BowAnim", "Models\\BowStance\\Attack01_BowAnim.fbx");
		AddFbxForAnimation("Attack01Maintain_BowAnim", "Models\\BowStance\\Attack01Maintain_BowAnim.fbx");
		AddFbxForAnimation("Attack01RepeatFire_BowAnim", "Models\\BowStance\\Attack01RepeatFire_BowAnim.fbx");
		AddFbxForAnimation("Attack01Start_BowAnim", "Models\\BowStance\\Attack01Start_BowAnim.fbx");
		AddFbxForAnimation("Attack02Maintain_BowAnim", "Models\\BowStance\\Attack02Maintain_BowAnim.fbx");
		AddFbxForAnimation("Attack02RepeatFire_BowAnim", "Models\\BowStance\\Attack02RepeatFire_BowAnim.fbx");
		AddFbxForAnimation("Attack02Start_BowAnim", "Models\\BowStance\\Attack02Start_BowAnim.fbx");
		AddFbxForAnimation("DashBackward_BowAnim", "Models\\BowStance\\DashBackward_BowAnim.fbx");
		AddFbxForAnimation("DashForward_BowAnim", "Models\\BowStance\\DashForward_BowAnim.fbx");

		AddFbxForAnimation("Walk_BowAnim", "Models\\BowStance\\Walk_BowAnim.fbx");
		AddFbxForAnimation("WalkBack_BowAnim", "Models\\BowStance\\WalkBack_BowAnim.fbx");
		AddFbxForAnimation("WalkRight_BowAnim", "Models\\BowStance\\WalkRight_BowAnim.fbx");
		AddFbxForAnimation("WalkLeft_BowAnim", "Models\\BowStance\\WalkLeft_BowAnim.fbx");
		AddFbxForAnimation("Idle_BowAnim", "Models\\BowStance\\Idle_BowAnim.fbx");
	}

	AnimatorController* controller = new AnimatorController();
	//*** AnimatorController ***//
	{

		controller->AddState("Attack01_BowAnim", animationClips["Attack01_BowAnim"].get());
		controller->AddState("Attack01Maintain_BowAnim", animationClips["Attack01Maintain_BowAnim"].get());
		controller->AddState("Attack01RepeatFire_BowAnim", animationClips["Attack01RepeatFire_BowAnim"].get());
		controller->AddState("Attack01Start_BowAnim", animationClips["Attack01Start_BowAnim"].get());
		controller->AddState("Attack02Maintain_BowAnim", animationClips["Attack02Maintain_BowAnim"].get());
		controller->AddState("Attack02RepeatFire_BowAnim", animationClips["Attack02RepeatFire_BowAnim"].get());
		controller->AddState("Attack02Start_BowAnim", animationClips["Attack02Start_BowAnim"].get());
		controller->AddState("DashBackward_BowAnim", animationClips["DashBackward_BowAnim"].get());
		controller->AddState("DashForward_BowAnim", animationClips["DashForward_BowAnim"].get());

		controller->AddParameterFloat("Speed");
		controller->AddParameterFloat("HoriSpeed");

		controller->AddState("Idle", animationClips["Idle_BowAnim"].get());
		controller->AddState("Walk", animationClips["Walk_BowAnim"].get());
		controller->AddState("WalkBack", animationClips["WalkBack_BowAnim"].get());
		controller->AddState("WalkRight", animationClips["WalkRight_BowAnim"].get());
		controller->AddState("WalkLeft", animationClips["WalkLeft_BowAnim"].get());

		controller->AddTransition("Idle", "Walk", TransitionCondition::CreateFloat("Speed", Greater, 0.1));
		controller->AddTransition("Idle", "WalkBack", TransitionCondition::CreateFloat("Speed", Less, -0.1));
		controller->AddTransition("Walk", "Idle", TransitionCondition::CreateFloat("Speed", Less, 0.1));
		controller->AddTransition("WalkBack", "Idle", TransitionCondition::CreateFloat("Speed", Greater, -0.1));

		controller->AddTransition("Idle", "WalkLeft", TransitionCondition::CreateFloat("HoriSpeed", Greater, 0.1));
		controller->AddTransition("Idle", "WalkRight", TransitionCondition::CreateFloat("HoriSpeed", Less, -0.1));
		controller->AddTransition("WalkLeft", "Idle", TransitionCondition::CreateFloat("HoriSpeed", Less, 0.1));
		controller->AddTransition("WalkRight", "Idle", TransitionCondition::CreateFloat("HoriSpeed", Greater, -0.1));
	}

	///*** Game Object ***///

	GameObject* mainCamera = CreateEmpty();
	{
		Camera* camera = mainCamera->AddComponent<Camera>();
		camera->main = camera;

		mainCamera->AddComponent<CameraController>();
	}

	UINT objCBIndex = gameObjects.size();

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

	auto ritem = CreateEmpty();
	{
		ritem->GetComponent<Transform>()->Scale({ 0.02, 0.02, 0.02 });
		ritem->GetComponent<Transform>()->Rotate({ 1, 0, 0 }, -90);
		ritem->GetComponent<Transform>()->position = { 0.0f, 0.0f, 0.0f };
		auto mesh = ritem->AddComponent<SkinnedMeshRenderer>()->mesh = geometries["ApprenticeSK"].get();
		auto renderer = ritem->GetComponent<SkinnedMeshRenderer>();
		for (auto& sm : mesh->DrawArgs)
			renderer->materials.push_back(0);

		ritem->TexTransform = MathHelper::Identity4x4();

		auto anim = ritem->AddComponent<Animator>();
		anim->controller = controller;
		anim->state = &controller->states[name[5]];
		anim->TimePos = Random::Range(0.0f, anim->controller->GetClipEndTime(anim->state));

		anim->state = &controller->states["Idle"];
		anim->TimePos = 0;
		ritem->AddComponent<CharacterController>();

		renderObjectsLayer[(int)RenderLayer::SkinnedOpaque][mesh].gameObjects.push_back(ritem);
	}


	{
		GameObject* grid = CreateEmpty();
		grid->GetComponent<Transform>()->position -= {128, 20, 128};
		auto mesh = grid->AddComponent<MeshFilter>()->mesh = gridMesh;
		grid->AddComponent<Renderer>()->materials.push_back(3);
		renderObjectsLayer[(int)RenderLayer::Opaque][gridMesh].gameObjects.push_back(grid);
	}

	{
		GameObject* network = CreateEmpty();
		network->AddComponent<Network>()->myCharacter = ritem;
	}
}