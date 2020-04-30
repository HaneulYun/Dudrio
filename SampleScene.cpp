#include "pch.h"
#include "SampleScene.h"

void SampleScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	AddTexture(0, "none", L"Textures\\none.dds");
	AddTexture(1, "polyArtTex", L"Textures\\PolyArtTex.dds");

	//*** Material ***//
	AddMaterial(0, "none", 0);
	AddMaterial(1, "PolyArt", 1, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);

	//*** Mesh ***//
	geometries["Image"] = Mesh::CreateQuad();
	geometries["Sphere"] = Mesh::CreateSphere();
	geometries["Plane"] = Mesh::CreatePlane();
	AddFbxForAnimation("ApprenticeSK", "Models\\modelTest.fbx");

	//*** Animation ***//
	AddFbxForAnimation("Walk_BowAnim", "Models\\BowStance\\Walk_BowAnim.fbx");
	AddFbxForAnimation("WalkBack_BowAnim", "Models\\BowStance\\WalkBack_BowAnim.fbx");
	AddFbxForAnimation("WalkRight_BowAnim", "Models\\BowStance\\WalkRight_BowAnim.fbx");
	AddFbxForAnimation("WalkLeft_BowAnim", "Models\\BowStance\\WalkLeft_BowAnim.fbx");
	AddFbxForAnimation("Idle_BowAnim", "Models\\BowStance\\Idle_BowAnim.fbx");

	//*** AnimatorController ***//
	AnimatorController* controller = new AnimatorController();
	{
		controller->AddParameterFloat("VelocityX");
		controller->AddParameterFloat("VelocityZ");

		controller->AddState("Idle", animationClips["Idle_BowAnim"].get());
		controller->AddState("Walk", animationClips["Walk_BowAnim"].get());
		controller->AddState("WalkBack", animationClips["WalkBack_BowAnim"].get());
		controller->AddState("WalkRight", animationClips["WalkRight_BowAnim"].get());
		controller->AddState("WalkLeft", animationClips["WalkLeft_BowAnim"].get());

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
	//	camera = camera->main = mainCamera->AddComponent<Camera>();
	//	mainCamera->AddComponent<HostCameraController>();
	}

	auto skyBox = CreateEmpty();
	{
		skyBox->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		skyBox->AddComponent<Renderer>()->materials.push_back(1);
		auto mesh = skyBox->AddComponent<MeshFilter>()->mesh = geometries["Sphere"].get();
		skyBox->layer = (int)RenderLayer::Sky;
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

	GameObject* ritem[MAX_USER];
	for (int i = 0; i < MAX_USER; ++i)
	{
		ritem[i] = CreateEmpty();
		auto model = ritem[i]->AddChild();
		{
			model->GetComponent<Transform>()->Scale({ 0.02, 0.02, 0.02 });
			model->GetComponent<Transform>()->Rotate({ 1, 0, 0 }, -90);
			auto mesh = model->AddComponent<SkinnedMeshRenderer>()->mesh = geometries["ApprenticeSK"].get();
			auto renderer = model->GetComponent<SkinnedMeshRenderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(1);
	
			auto anim = model->AddComponent<Animator>();
			anim->controller = controller;
			anim->state = &controller->states["Idle"];
			anim->TimePos = 0;
	
			ritem[i]->AddComponent<CharacterController>()->anim= anim;
		}
	}
	auto cameraOffset = ritem[0]->AddChild();
	{
		cameraOffset->transform->position = { 0, 3, -6 };
		camera = camera->main = cameraOffset->AddComponent<Camera>();
		cameraOffset->AddComponent<CameraController>();
	}

	auto grid = CreateEmpty();
	{
		grid->AddComponent<MeshFilter>()->mesh = geometries["Plane"].get();
		grid->AddComponent<Renderer>()->materials.push_back(0);
	}

	auto network = CreateEmpty();
	{
		network->AddComponent<Network>()->myCharacter = ritem[0];
		for (int i = 0; i < MAX_USER - 1; ++i)
		{
			network->GetComponent<Network>()->otherCharacter[i] = ritem[i + 1];
		}
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