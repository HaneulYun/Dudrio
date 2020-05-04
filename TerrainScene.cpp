#include "pch.h"
#include "TerrainScene.h"

BuildManager* BuildManager::buildManager{ nullptr };
ButtonManager* ButtonManager::buttonManager{ nullptr };

void TerrainScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	{
		AddTexture(0, "none", L"Textures\\none.dds");
		AddTexture(1, "ground", L"Textures\\grass.dds");
		AddTexture(2, "grass", L"Textures\\grass01.dds");
		AddTexture(3, "house01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_04_D.dds");
		AddTexture(4, "house02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_09_D.dds");
		AddTexture(5, "material_01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_01_D.dds");

	}


	//*** Material ***//
	{
		AddMaterial(0, "none", 0);
		AddMaterial(1, "ground", 1, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(2, "grass", 2, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
		AddMaterial(3, "house01", 3, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(4, "house02", 4, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(5, "material_01", 5, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
	}

	//*** Mesh ***//
	{
		geometries["Image"] = Mesh::CreateQuad();
		geometries["Sphere"] = Mesh::CreateSphere();
		AddFbxForAnimation("SM_House_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
		AddFbxForAnimation("SM_House_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02.FBX");
		AddFbxForAnimation("SM_Cart_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var01.FBX");
		AddFbxForAnimation("SM_Cart_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var02.FBX");
		AddFbxForAnimation("SM_Barrel", "Assets\\AdvancedVillagePack\\Meshes\\SM_Barrel.FBX");
		AddFbxForAnimation("SM_Fence_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var01.FBX");
		AddFbxForAnimation("SM_Fence_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var02.FBX");
		AddFbxForAnimation("SM_Fence_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var03.FBX");
		AddFbxForAnimation("SM_Fence_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var04.FBX");
	}

	CHeightMapImage* m_pHeightMapImage = new CHeightMapImage(L"Texture\\heightMap.raw", 257, 257, { 1.0f, 0.1f, 1.0f });
	CHeightMapGridMesh* gridMesh = new CHeightMapGridMesh(0, 0, 257, 257, { 1, 1, 1 }, { 1, 1, 0, 1 }, m_pHeightMapImage);

	///*** Game Object ***///

	GameObject* mainCamera = CreateEmpty();
	{
		camera = camera->main = mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<HostCameraController>();
	}

	{
		auto ritem = CreateEmpty();
		ritem->GetComponent<Transform>()->Scale({ 5000.0f, 5000.0f, 5000.0f });
		auto mesh = ritem->AddComponent<MeshFilter>()->mesh = geometries["Sphere"].get();
		auto renderer = ritem->AddComponent<Renderer>();
		for (auto& sm : mesh->DrawArgs)
			renderer->materials.push_back(5);

		ritem->layer = (int)RenderLayer::Sky;
	}


	GameObject* grid = CreateEmpty();
	{
		grid->GetComponent<Transform>()->position -= {128, 10, 128};
		auto mesh = grid->AddComponent<MeshFilter>()->mesh = gridMesh;
		grid->AddComponent<Renderer>()->materials.push_back(1);
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
	}

	// billboard points
	{
		struct TreeSpriteVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT2 Size;
			XMFLOAT3 look;
		};
		std::vector<TreeSpriteVertex> vertices;
		float sizex = 1, sizey = 1;
		const int width = 256, length = 256;
		vertices.reserve(width* length * 2.0);
		for (float i = 0; i < width; i += 0.5f)
		{
			for (float j = 0; j < length; j += 0.5f)
			{
				TreeSpriteVertex v;
				v.Pos = XMFLOAT3(i, gridMesh->OnGetHeight(i, j, m_pHeightMapImage) + sizey / 2, j);
				v.Size = XMFLOAT2(sizex, sizey);
				v.look = XMFLOAT3(MathHelper::RandF(0.0f, 1.0f), 0.0f, MathHelper::RandF(0.0f, 1.0f));
				vertices.push_back(v);
			}
		}

		auto geo = std::make_unique<Mesh>();
		const UINT vbByteSize = (UINT)vertices.size() * sizeof(TreeSpriteVertex);

		geo->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU);
		CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		auto device = Graphics::Instance()->device;
		auto commandList = Graphics::Instance()->commandList;

		geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device.Get(), commandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

		geo->VertexByteStride = sizeof(TreeSpriteVertex);
		geo->VertexBufferByteSize = vbByteSize;

		SubmeshGeometry submesh;
		submesh.IndexCount = vertices.size();
		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;

		geo->DrawArgs["submesh"] = submesh;
		geometries["Grass"] = std::move(geo);


		GameObject* billboards = CreateEmpty();
		billboards->GetComponent<Transform>()->position -= {128, 10, 128};
		auto mesh = billboards->AddComponent<MeshFilter>()->mesh = geometries["Grass"].get();
		billboards->AddComponent<Renderer>()->materials.push_back(2);
		billboards->layer = (int)RenderLayer::Grass;

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

	// Build Button
	
	auto BSButton00 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton00, false));
	{
		auto rectTransform = BSButton00->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -130;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton00->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var01"].get(), 3, 0.02);
			});
		{
			auto textobject = BSButton00->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"랜드마크1";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton00->SetActive(false);
	}
	auto BSButton01 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton01, false));
	{
		auto rectTransform = BSButton01->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -70;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton01->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var02"].get(), 4, 0.02);
			});
		{
			auto textobject = BSButton01->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"랜드마크2";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton01->SetActive(false);
	}
	auto BSButton02 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton02, false));
	{
		auto rectTransform = BSButton02->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -10;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton02->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var02"].get(), 4, 0.02);
			});
		{
			auto textobject = BSButton02->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"랜드마크3";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton02->SetActive(false);
	}
	auto BuildingSelectButton00 = CreateImage();
	{
		auto rectTransform = BuildingSelectButton00->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -130;
		rectTransform->posY = 10;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BuildingSelectButton00->AddComponent<Button>()->AddEvent(
			[](void*) {
				ButtonManager::buttonManager->SelectButton(0);
				ButtonManager::buttonManager->SelectButton(1);
				ButtonManager::buttonManager->SelectButton(2);
			});
		{
			auto textobject = BuildingSelectButton00->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"랜드마크";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto BSButton10 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton10, false));
	{
		auto rectTransform = BSButton10->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -130;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton10->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var01"].get(), 3, 0.02);
			});
		{
			auto textobject = BSButton10->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"집1";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton10->SetActive(false);
	}
	auto BSButton11 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton11, false));
	{
		auto rectTransform = BSButton11->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -70;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton11->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var02"].get(), 4, 0.02);
			});
		{
			auto textobject = BSButton11->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"집2";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton11->SetActive(false);
	}
	auto BSButton12 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton12, false));
	{
		auto rectTransform = BSButton12->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -10;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton12->AddComponent<Button>()->AddEvent(
			[](void*) {
				
			});
		{
			auto textobject = BSButton12->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"없음";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton12->SetActive(false);
	}
	auto BuildingSelectButton01 = CreateImage();
	{
		auto rectTransform = BuildingSelectButton01->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -70;
		rectTransform->posY = 10;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BuildingSelectButton01->AddComponent<Button>()->AddEvent(
			[](void*) {
				ButtonManager::buttonManager->SelectButton(3);
				ButtonManager::buttonManager->SelectButton(4);
				ButtonManager::buttonManager->SelectButton(5);
			});
		{
			auto textobject = BuildingSelectButton01->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"주거건물";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto BSButton20 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton20, false));
	{
		auto rectTransform = BSButton20->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -130;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton20->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var01"].get(), 3, 0.02);
			});
		{
			auto textobject = BSButton20->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"없음";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton20->SetActive(false);
	}
	auto BSButton21 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton21, false));
	{
		auto rectTransform = BSButton21->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -70;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton21->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var02"].get(), 4, 0.02);
			});
		{
			auto textobject = BSButton21->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"없음";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton21->SetActive(false);
	}
	auto BSButton22 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton22, false));
	{
		auto rectTransform = BSButton22->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -10;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton22->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_House_Var02"].get(), 4, 0.02);
			});
		{
			auto textobject = BSButton22->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"없음";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton22->SetActive(false);
	}
	auto BuildingSelectButton02 = CreateImage();
	{
		auto rectTransform = BuildingSelectButton02->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -10;
		rectTransform->posY = 10;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BuildingSelectButton02->AddComponent<Button>()->AddEvent(
			[](void*) {
				ButtonManager::buttonManager->SelectButton(6);
				ButtonManager::buttonManager->SelectButton(7);
				ButtonManager::buttonManager->SelectButton(8);
			});
		{
			auto textobject = BuildingSelectButton02->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"테마건물";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto BSButton30 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton30, false));
	{
		auto rectTransform = BSButton30->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -130;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton30->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var01"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton30->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리1";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton30->SetActive(false);
	}
	auto BSButton31 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton31, false));
	{
		auto rectTransform = BSButton31->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -70;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton31->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var02"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton31->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리2";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton31->SetActive(false);
	}
	auto BSButton32 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton32, false));
	{
		auto rectTransform = BSButton32->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -10;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton32->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var03"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton32->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리3";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton32->SetActive(false);
	}
	auto BSButton33 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton33, false));
	{
		auto rectTransform = BSButton33->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = 50;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton33->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var04"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton33->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리4";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton33->SetActive(false);
	}
	auto BuildingSelectButton03 = CreateImage();
	{
		auto rectTransform = BuildingSelectButton03->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = 50;
		rectTransform->posY = 10;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BuildingSelectButton03->AddComponent<Button>()->AddEvent(
			[](void*) {
				ButtonManager::buttonManager->SelectButton(9);
				ButtonManager::buttonManager->SelectButton(10);
				ButtonManager::buttonManager->SelectButton(11);
				ButtonManager::buttonManager->SelectButton(12);
			});
		{
			auto textobject = BuildingSelectButton03->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"조경";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto BSButton40 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton40, false));
	{
		auto rectTransform = BSButton40->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -130;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton40->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var01"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton40->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리1";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton40->SetActive(false);
	}
	auto BSButton41 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton41, false));
	{
		auto rectTransform = BSButton41->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -70;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton41->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var02"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton41->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리2";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton41->SetActive(false);
	}
	auto BSButton42 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton42, false));
	{
		auto rectTransform = BSButton42->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = -10;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton42->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var03"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton42->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리3";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton42->SetActive(false);
	}
	auto BSButton43 = CreateImage();
	ButtonManager::buttonManager->buttons.push_back(std::make_pair(BSButton43, false));
	{
		auto rectTransform = BSButton43->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = 50;
		rectTransform->posY = 70;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BSButton43->AddComponent<Button>()->AddEvent(
			[](void*) {
				//BuildManager::buildManager->SelectModel(Scene::scene->geometries["SM_Fence_Var04"].get(), 5, 0.02);
			});
		{
			auto textobject = BSButton43->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"울타리4";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
		BSButton43->SetActive(false);
	}
	auto BuildingSelectButton04 = CreateImage();
	{
		auto rectTransform = BuildingSelectButton04->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = 110;
		rectTransform->posY = 10;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BuildingSelectButton04->AddComponent<Button>()->AddEvent(
			[](void*) {
				ButtonManager::buttonManager->SelectButton(13);
				ButtonManager::buttonManager->SelectButton(14);
				ButtonManager::buttonManager->SelectButton(15);
				ButtonManager::buttonManager->SelectButton(16);
			});
		{
			auto textobject = BuildingSelectButton04->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"소품";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}
}