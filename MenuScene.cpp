#include "pch.h"
#include "MenuScene.h"

void MenuScene::BuildObjects()
{
	///*** Asset ***///
	LoadTextureAsset();
	LoadMaterialAsset();
	LoadMeshAsset();

	LoadPrefab();

	////*** AudioClip ***//
	ASSET AddAudioClip("bgm_menu", "Sounds\\bgm_menu.mp3");

	///*** Game Object ***///
	auto mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
	}

	auto soundBox = CreateEmpty();
	{
		auto audioSource = soundBox->AddComponent<AudioSource>();
		audioSource->clip = ASSET AUDIO_CLIP("bgm_menu");
		audioSource->loop = true;
	}

	auto background = CreateImage();
	{
		background->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("menuBackgroundMat");

		auto rectTransform = background->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 1, 1 };

		auto dudrioText = background->AddChildUI(CreateImagePrefab());
		{
			dudrioText->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("dudrio");

			auto rectTransform = dudrioText->GetComponent<RectTransform>();
			rectTransform->setAnchorAndPivot(0.5, 1);
			rectTransform->setPosAndSize(0, -200, 650, 150);
		}


		auto GuestSceneButton = CreateImage();
		{
			auto rectTransform = GuestSceneButton->GetComponent<RectTransform>();
			rectTransform->setAnchorAndPivot(0.5, 1);
			rectTransform->setPosAndSize(0, -370, 160, 60);

			GuestSceneButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					SceneManager::LoadScene("GuestMenuScene");
				});
			GuestSceneButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("start_guest");
		}

		auto HostSceneButton = CreateImage();
		{
			auto rectTransform = HostSceneButton->GetComponent<RectTransform>();
			rectTransform->setAnchorAndPivot(0.5, 1);
			rectTransform->setPosAndSize(0, -440, 160, 60);

			HostSceneButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					SceneManager::LoadScene("HostMenuScene");
				});
			HostSceneButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("start_host");
		}

	}
}
