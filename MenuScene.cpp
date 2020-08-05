#include "pch.h"
#include "MenuScene.h"

void MenuScene::BuildObjects()
{
	///*** Asset ***///
	LoadTextureAsset();
	LoadMaterialAsset();
	LoadMeshAsset();

	LoadPrefab();

	ASSET AddTexture("menuBackgroundTex", L"Textures\\menu\\background.dds");
	ASSET AddMaterial("menuBackgroundMat", ASSET TEXTURE("menuBackgroundTex"), 0, 0, 0, { 0.8, 0.8, 0.8, 1 });

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

		auto text = background->AddComponent<Text>();
		text->text = L"두드리오\n\n\n";
		text->font = L"메이플스토리";
		text->fontSize = 120;
		text->color = { 1, 1, 1, 1 };
		text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;


		auto GuestSceneButton = CreateImage();
		{
			auto rectTransform = GuestSceneButton->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = -10;
			rectTransform->posY = 20;
			rectTransform->width = 150;
			rectTransform->height = 30;

		GuestSceneButton->AddComponent<Button>()->AddEvent(
			[](void*) {
				SceneManager::LoadScene("GuestMenuScene");
			});
		{
			auto textobject = GuestSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Guest Scene";
				text->font = L"메이플스토리";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
			GuestSceneButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
		}

		auto HostSceneButton = CreateImage();
		{
			auto rectTransform = HostSceneButton->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = -10;
			rectTransform->posY = -60;
			rectTransform->width = 150;
			rectTransform->height = 30;

			HostSceneButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					SceneManager::LoadScene("HostMenuScene");
				});
			{
				auto textobject = HostSceneButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Host Scene";
				text->font = L"메이플스토리";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		auto particleSceneButton = CreateImage();
		{
			auto rectTransform = particleSceneButton->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = -10;
			rectTransform->posY = -140;
			rectTransform->width = 150;
			rectTransform->height = 30;

			particleSceneButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					SceneManager::LoadScene("ParticleScene");
				});
			{
				auto textobject = particleSceneButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Particle Scene";
				text->font = L"메이플스토리";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

		auto TestSceneButton = CreateImage();
		{
			auto rectTransform = TestSceneButton->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = -10;
			rectTransform->posY = -180;
			rectTransform->width = 150;
			rectTransform->height = 30;

			TestSceneButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					SceneManager::LoadScene("TestScene");
				});
			{
				auto textobject = TestSceneButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Test Scene";
				text->font = L"메이플스토리";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}

	}
}
