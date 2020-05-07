#include "pch.h"
#include "AnimationScene.h"

void AnimationScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	ASSET AddTexture("none", L"Textures\\none.dds");
	ASSET AddTexture("polyArtTex", L"Textures\\PolyArtTex.dds");

	//*** Material ***//
	ASSET AddMaterial("none", ASSET TEXTURE("none"));
	ASSET AddMaterial("PolyArt", ASSET TEXTURE("polyArtTex"), -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);

	//*** Mesh ***//
	//ASSET AddMesh("Image", Mesh::CreateQuad());
	ASSET AddMesh("Plane", Mesh::CreatePlane());
	//ASSET AddMesh("Sphere", Mesh::CreateSphere());
	ASSET AddFbxForAnimation("ApprenticeSK", "Models\\modelTest.fbx");

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
		
	}

	auto skyBox = CreateEmpty();
	{
		skyBox->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		skyBox->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("PolyArt"));
		auto mesh = skyBox->AddComponent<MeshFilter>()->mesh = ASSET MESH("Sphere");
		skyBox->layer = (int)RenderLayer::Sky;
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

	auto grid = CreateEmpty();
	{
		grid->AddComponent<MeshFilter>()->mesh = ASSET MESH("Plane");
		grid->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));
	}

	auto guestPlayer = CreateEmpty();
	{
		auto model = guestPlayer->AddChild();
		{
			model->GetComponent<Transform>()->Scale({ 0.01, 0.01, 0.01 });
			model->GetComponent<Transform>()->Rotate({ 1, 0, 0 }, -90);
			auto mesh = model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
			auto renderer = model->GetComponent<SkinnedMeshRenderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(ASSET MATERIAL("PolyArt"));

			auto animator = model->AddComponent<Animator>();
			animator->controller = controller;
			animator->state = &controller->states["Idle"];
			animator->TimePos = 0;

			guestPlayer->AddComponent<GuestController>()->animator = animator;
		}
		auto cameraOffset = guestPlayer->AddChild();
		{
			cameraOffset->transform->position = { 0, 3, -6 };
			camera = camera->main = cameraOffset->AddComponent<Camera>();
			cameraOffset->AddComponent<CameraController>();
		}
	}

}
