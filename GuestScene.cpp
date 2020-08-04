#include "pch.h"
#include "GuestScene.h"

GuestGameWorld* GuestGameWorld::gameWorld;

void GuestScene::BuildObjects()
{
	LoadTextureAsset();
	LoadMaterialAsset();
	LoadMeshAsset();

	LoadPrefab();

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

	auto simPrefab = CreateEmptyPrefab();
	{
		auto model = simPrefab->AddChild();
		{
			model->GetComponent<Transform>()->Rotate({ 1, 0, 0 }, -90);
			model->AddComponent<SkinnedMeshRenderer>()->mesh = ASSET MESH("ApprenticeSK");
			model->GetComponent<SkinnedMeshRenderer>()->materials.push_back(ASSET MATERIAL("PolyArt"));
		}

		auto behavior = simPrefab->AddComponent<CharacterMovingBehavior>();
		auto anim = simPrefab->AddComponent<Animator>();
		anim->controller = controller;
		anim->state = &controller->states["Idle"];
		anim->TimePos = 0;
		behavior->anim = anim;
	}

	auto connect_inform = GuestInformConnector::connector->selected_room;
	float TerrainSize = connect_inform->terrain_size;
	float frequency = connect_inform->frequency;
	int octaves = connect_inform->octaves;
	int seed = connect_inform->seed;
	// 지형 생성
	TerrainGenerator* terrainGenerator = new TerrainGenerator(TerrainSize, TerrainSize);
	string fileName = terrainGenerator->createHeightMap(frequency, octaves, seed, (char*)"square");
	delete terrainGenerator;

	GameObject* terrain = CreateEmpty();
	auto terrainData = terrain->AddComponent<Terrain>();
	simPrefab->GetComponent<CharacterMovingBehavior>()->heightmap = &terrainData->terrainData;
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

	auto sea = CreateEmpty();
	{
		sea->transform->position = { 500, 1, 500 };
		sea->transform->Scale({ 10000, 1, 10000 });
		sea->AddComponent<MeshFilter>()->mesh = ASSET MESH("Plane");
		sea->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("sea"));
	}
	auto bottom = CreateEmpty();
	{
		bottom->transform->position = { 500, 1, 500 };
		bottom->transform->Scale({ 10000, 1, 10000 });
		bottom->transform->Rotate({ 1, 0, 0 }, 180);
		bottom->AddComponent<MeshFilter>()->mesh = ASSET MESH("Plane");
		bottom->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("none"));
	}

	auto object = CreateUI();
	{
		auto buildingBuilder = object->AddComponent<BuildingBuilder>();
		buildingBuilder->serializeBuildings();
		buildingBuilder->terrain = terrainData;
		buildingBuilder->terrainNodeData = terrainNodeData;

		GuestGameWorld* gameWorld = object->AddComponent<GuestGameWorld>();
		gameWorld->sun = directionalLight;
	}

	auto network = CreateEmpty();
	GuestNetwork* guestNetwork = network->AddComponent<GuestNetwork>();
	{
		guestNetwork->simsPrefab = simPrefab;

		auto player = guestNetwork->myCharacter = Duplicate(simPrefab);
		player->GetComponent<Transform>()->position = { 540.0, 0.0, 540.0 };
		player->AddComponent<CharacterController>();
		strcpy_s(player->GetComponent<CharacterMovingBehavior>()->name, GuestInformConnector::connector->name);
		GuestNetwork::network = guestNetwork;

		auto cameraOffset = player->AddChild();
		{
			camera = camera->main = cameraOffset->AddComponent<Camera>();
			cameraOffset->transform->position = { 0, 2, -3 };
		}
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
				GuestNetwork::network->PressButton();
			});
		{
			auto textobject = ServerButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Connect";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

			guestNetwork->connectButtonText = text;
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
		}
	}

}