#include "pch.h"
#include "MenuScene.h"

void MenuScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	ASSET AddTexture("none", L"Textures\\none.dds");
	ASSET AddTexture("menuBackgroundTex", L"Textures\\menu\\background.dds");

	//*** Material ***//
	ASSET AddMaterial("none", ASSET TEXTURE("none"));
	ASSET AddMaterial("gray", ASSET TEXTURE("none"), -1, { 0.5, 0.5, 0.5, 0.5 });
	ASSET AddMaterial("menuBackgroundMat", ASSET TEXTURE("menuBackgroundTex"), -1, { 0.8, 0.8, 0.8, 1 });

	//*** Mesh ***//
	ASSET AddMesh("Image", Mesh::CreateQuad());
	ASSET AddMesh("Sphere", Mesh::CreateSphere());

	///*** Game Object ***///

	auto mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		//mainCamera->AddComponent<CameraController>();
	}

	auto background = CreateImage();
	{
		background->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("menuBackgroundMat");

		auto rectTransform = background->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 1, 1 };

		auto text = background->AddComponent<Text>();
		text->text = L"�ε帮��\n\n\n";
		text->font = L"�����ý��丮";
		text->fontSize = 120;
		text->color = { 1, 1, 1, 1 };
		text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		textObjects.push_back(background);


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
				Debug::Log("�̰� �ǳ�;;\n");
				SceneManager::LoadScene("GuestScene");
			});
		{
			auto textobject = GuestSceneButton->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Guest Scene";
				text->font = L"�����ý��丮";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				textObjects.push_back(textobject);
			}
			GuestSceneButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
		}

		auto materialSceneButton = CreateImage();
		{
			auto rectTransform = materialSceneButton->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = -10;
			rectTransform->posY = -20;
			rectTransform->width = 150;
			rectTransform->height = 30;

			materialSceneButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					//SceneManager::LoadScene("MaterialScene");
				});
			{
				auto textobject = materialSceneButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Material Scene";
				text->font = L"�����ý��丮";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				textObjects.push_back(textobject);
			}
			materialSceneButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("none");
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
					SceneManager::LoadScene("HostScene");
				});
			{
				auto textobject = HostSceneButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Host Scene";
				text->font = L"�����ý��丮";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				textObjects.push_back(textobject);
			}
		}

		auto animationSceneButton = CreateImage();
		{
			auto rectTransform = animationSceneButton->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0.5, 0.5 };
			rectTransform->anchorMax = { 0.5, 0.5 };
			rectTransform->pivot = { 0.5, 0.5 };
			rectTransform->posX = -10;
			rectTransform->posY = -100;
			rectTransform->width = 150;
			rectTransform->height = 30;

			animationSceneButton->AddComponent<Button>()->AddEvent(
				[](void*) {
					SceneManager::LoadScene("AnimationScene");
				});
			{
				auto textobject = animationSceneButton->AddChildUI();
				auto rectTransform = textobject->GetComponent<RectTransform>();
				rectTransform->anchorMin = { 0, 0 };
				rectTransform->anchorMax = { 1, 1 };

				Text* text = textobject->AddComponent<Text>();
				text->text = L"Animation Scene";
				text->font = L"�����ý��丮";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				textObjects.push_back(textobject);
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
				text->font = L"�����ý��丮";
				text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				textObjects.push_back(textobject);
			}
		}
	}
}
