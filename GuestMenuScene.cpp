#include "pch.h"
#include "GuestMenuScene.h"

GuestInformConnector* GuestInformConnector::connector{ nullptr };
wchar_t GuestInformConnector::name[MAX_ID_LEN + 1];
int GuestInformConnector::characterIndex;
RoomInfo* GuestInformConnector::selected_room;


void GuestMenuScene::BuildObjects()
{
	//*** AnimatorController ***//
	AnimatorController* controller = new AnimatorController();
	{
		controller->AddParameterFloat("VelocityX");
		controller->AddParameterFloat("VelocityZ");

		controller->AddState("Idle", ASSET animationClips["Idle"].get());
		controller->AddState("Walk", ASSET animationClips["Walk"].get());
		controller->AddState("WalkBack", ASSET animationClips["WalkBack"].get());
		controller->AddState("WalkRight", ASSET animationClips["WalkRight"].get());
		controller->AddState("WalkLeft", ASSET animationClips["WalkLeft"].get());

		controller->AddTransition("Idle", "Walk", TransitionCondition::CreateFloat("VelocityZ", Greater, 0.3));
		controller->AddTransition("Idle", "WalkBack", TransitionCondition::CreateFloat("VelocityZ", Less, -0.3));
		controller->AddTransition("Walk", "Idle", TransitionCondition::CreateFloat("VelocityZ", Less, 0.3));
		controller->AddTransition("WalkBack", "Idle", TransitionCondition::CreateFloat("VelocityZ", Greater, -0.3));

		controller->AddTransition("Idle", "WalkLeft", TransitionCondition::CreateFloat("VelocityX", Greater, 0.3));
		controller->AddTransition("Idle", "WalkRight", TransitionCondition::CreateFloat("VelocityX", Less, -0.3));
		controller->AddTransition("WalkLeft", "Idle", TransitionCondition::CreateFloat("VelocityX", Less, 0.3));
		controller->AddTransition("WalkRight", "Idle", TransitionCondition::CreateFloat("VelocityX", Greater, -0.3));
	}
	///*** Asset ***///
	//*** Texture ***//

	//*** Material ***//

	//*** Mesh ***//

	///*** Game Object ***///
	{
		auto skybox = CreateEmpty();
		skybox->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		skybox->AddComponent<MeshFilter>()->mesh = ASSET MESH("Sphere");
		skybox->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));
		skybox->layer = (int)RenderLayer::Sky;
	}

	auto directionalLight = CreateEmpty();
	{
		directionalLight->transform->Rotate({ 1, 0, 0 }, 60);
		auto light = directionalLight->AddComponent<Light>();
		light->Strength = { 0.9f, 0.8f, 0.7f };
		light->shadowType = Light::Shadows;

		environment.sunSources = light;
	}

	auto mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		mainCamera->transform->position = { 1.3, 1.5, -3 };
		//mainCamera->AddComponent<CameraController>();
	}

	GameObject* simPrefab[21];
	for (int i = 0; i < 21; ++i) {
		simPrefab[i] = CreateEmptyPrefab();
		{
			auto model = simPrefab[i]->AddChild();
			{
				model->GetComponent<Transform>()->Rotate({ 1, 0, 0 }, -90);
				if (i == 0)		 model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
				else if (i == 1) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("BattleMageSK");
				else if (i == 2) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("CommonerSK");
				else if (i == 3) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("CountSK");
				else if (i == 4) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("DarkKnightSK");
				else if (i == 5) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("DukeSK");
				else if (i == 6) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("EliteSK");
				else if (i == 7) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("EngineerSK");
				else if (i == 8) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ExecutionerSK");
				else if (i == 9) model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("FootmanSK");
				else if (i == 10)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("HeavyKnightSK");
				else if (i == 11)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ImmortalSK");
				else if (i == 12)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("IronMaskSK");
				else if (i == 13)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("KnightSK");
				else if (i == 14)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("MageSK");
				else if (i == 15)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("PrinceSK");
				else if (i == 16)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ShinobiSK");
				else if (i == 17)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("SoldierSK");
				else if (i == 18)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("TemplarSK");
				else if (i == 19)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("WarriorSK");
				else if (i == 20)model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("WizardSK");
				model->GetComponent<SkinnedMeshRenderer>()->materials.push_back(ASSET MATERIAL("PolyArt"));
			}

			simPrefab[i]->GetComponent<Transform>()->Rotate({ 0, 1, 0 }, 170);
			auto anim = simPrefab[i]->AddComponent<Animator>();
			anim->controller = controller;
			anim->state = &controller->states["Idle"];
			anim->TimePos = 0;
		}
	}

	auto object = CreateEmpty();
	GuestInformConnector* informConnector = object->AddComponent<GuestInformConnector>();
	{
		for (int i = 0; i < 21; ++i) {
			informConnector->characters[i] = simPrefab[i];
		}
		informConnector->curCharacter = Duplicate(simPrefab[0]);
		GuestInformConnector::connector = informConnector;
	}

	auto background = CreateImage();
	{
		background->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("menuBackgroundMat");

		auto rectTransform = background->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 1, 1 };
	}
	informConnector->background = background;

	auto hostButtonPrefab = CreateImagePrefab();
	{
		auto rt = hostButtonPrefab->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(0, 1);
		rt->setPosAndSize(50, -50, 100, 40);

		{
			auto textobject = hostButtonPrefab->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Guest Scene";
			text->fontSize = 20;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		hostButtonPrefab->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
		informConnector->buttonPrefab = hostButtonPrefab;
	}
}
