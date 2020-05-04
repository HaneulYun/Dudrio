#include "pch.h"
#include "TerrainScene.h"

BuildManager* BuildManager::buildManager{ nullptr };
ButtonManager* ButtonManager::buttonManager{ nullptr };

GameObject* TerrainScene::CreateTextButton()
{
	GameObject* button = CreateImage();
	{
		auto rectTransform = button->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.5, 0 };
		rectTransform->anchorMax = { 0.5, 0 };
		rectTransform->pivot = { 0.5, 0 };
		rectTransform->posX = 0;
		rectTransform->posY = 0;
		rectTransform->width = 40;
		rectTransform->height = 40;

		{
			auto textobject = button->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"X";
			text->fontSize = 10;
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}
	return button;
}

GameObject* TerrainScene::CreateButtonList()
{
	GameObject* button = CreateImage();
	{
		auto rectTransform = button->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = 40;
		rectTransform->posY = 40;
		rectTransform->width = CyanFW::Instance()->GetWidth() - 80;
		rectTransform->height = 60;
	}
	button->GetComponent<Renderer>()->materials[0] = 1;
	button->SetActive(false);

	for (int k = 0; k < 10; ++k)
	{
		GameObject* button_building = CreateTextButton();
		button_building->SetActive(false);
		auto rectTransform = button_building->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0.05f + 0.1f * k, 0.1 };
		rectTransform->anchorMax = { 0.1f * k + 0.09f, 0.9 };
		button->AddChild(button_building);
	}
	return button;
}



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
		AddTexture(6, "material_03", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_03_D.dds");
		AddTexture(7, "TreeLeafs", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeLeafs_D.dds");
	}


	//*** Material ***//
	{
		AddMaterial(0, "none", 0);
		AddMaterial(1, "ground", 1, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(2, "grass", 2, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
		AddMaterial(3, "house01", 3, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(4, "house02", 4, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(5, "material_01", 5, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(6, "material_03", 6, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(7, "TreeLeafs", 7, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
	}

	//*** Mesh ***//
	{
		geometries["Image"] = Mesh::CreateQuad();
		geometries["Sphere"] = Mesh::CreateSphere();
		AddFbxForAnimation("SM_Well", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well.FBX");
		AddFbxForAnimation("SM_Well_Extra02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra02.FBX");
		AddFbxForAnimation("SM_Well_Extra03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra03.FBX");
		AddFbxForAnimation("SM_House_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
		AddFbxForAnimation("SM_House_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02.FBX");
		AddFbxForAnimation("SM_House_Var02_Extra", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02_Extra.FBX");
		AddFbxForAnimation("SM_Tree_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var01.FBX");
		AddFbxForAnimation("SM_Tree_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var02.FBX");
		AddFbxForAnimation("SM_Tree_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var03.FBX");
		AddFbxForAnimation("SM_Tree_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var04.FBX");
		AddFbxForAnimation("SM_Tree_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var05.FBX");
		AddFbxForAnimation("SM_Cart_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var01.FBX");
		AddFbxForAnimation("SM_Cart_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var02.FBX");
		AddFbxForAnimation("SM_Barrel", "Assets\\AdvancedVillagePack\\Meshes\\SM_Barrel.FBX");
		//AddFbxForAnimation("SM_Fence_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var01.FBX");
		//AddFbxForAnimation("SM_Fence_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var02.FBX");
		//AddFbxForAnimation("SM_Fence_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var03.FBX");
		//AddFbxForAnimation("SM_Fence_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var04.FBX");
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

	//// billboard points
	//{
	//	struct TreeSpriteVertex
	//	{
	//		XMFLOAT3 Pos;
	//		XMFLOAT2 Size;
	//		XMFLOAT3 look;
	//	};
	//	std::vector<TreeSpriteVertex> vertices;
	//	float sizex = 1, sizey = 1;
	//	const int width = 256, length = 256;
	//	vertices.reserve(width* length * 2.0);
	//	for (float i = 0; i < width; i += 0.5f)
	//	{
	//		for (float j = 0; j < length; j += 0.5f)
	//		{
	//			TreeSpriteVertex v;
	//			v.Pos = XMFLOAT3(i, gridMesh->OnGetHeight(i, j, m_pHeightMapImage) + sizey / 2, j);
	//			v.Size = XMFLOAT2(sizex, sizey);
	//			v.look = XMFLOAT3(MathHelper::RandF(0.0f, 1.0f), 0.0f, MathHelper::RandF(0.0f, 1.0f));
	//			vertices.push_back(v);
	//		}
	//	}
	//
	//	auto geo = std::make_unique<Mesh>();
	//	const UINT vbByteSize = (UINT)vertices.size() * sizeof(TreeSpriteVertex);
	//
	//	geo->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	//	D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU);
	//	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//	auto device = Graphics::Instance()->device;
	//	auto commandList = Graphics::Instance()->commandList;
	//
	//	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device.Get(), commandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);
	//
	//	geo->VertexByteStride = sizeof(TreeSpriteVertex);
	//	geo->VertexBufferByteSize = vbByteSize;
	//
	//	SubmeshGeometry submesh;
	//	submesh.IndexCount = vertices.size();
	//	submesh.StartIndexLocation = 0;
	//	submesh.BaseVertexLocation = 0;
	//
	//	geo->DrawArgs["submesh"] = submesh;
	//	geometries["Grass"] = std::move(geo);
	//
	//
	//	GameObject* billboards = CreateEmpty();
	//	billboards->GetComponent<Transform>()->position -= {128, 10, 128};
	//	auto mesh = billboards->AddComponent<MeshFilter>()->mesh = geometries["Grass"].get();
	//	billboards->AddComponent<Renderer>()->materials.push_back(2);
	//	billboards->layer = (int)RenderLayer::Grass;
	//
	//}

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

	GameObject* buttons_BuildingType[6];
	std::vector<GameObject*> butttons_BuildingList[6];

	for (int i = 0; i < 6; ++i)
	{
		buttons_BuildingType[i] = CreateTextButton();
		buttons_BuildingType[i]->GetComponent<RectTransform>()->posX = -80 + 40 * i;
	}
	buttons_BuildingType[0]->children.front()->GetComponent<Text>()->text = L"랜드\n마크";
	buttons_BuildingType[0]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::LandMark); });
	buttons_BuildingType[1]->children.front()->GetComponent<Text>()->text = L"주거\n건물";
	buttons_BuildingType[1]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::House); });
	buttons_BuildingType[2]->children.front()->GetComponent<Text>()->text = L"테마\n건물";
	buttons_BuildingType[2]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Theme); });
	buttons_BuildingType[3]->children.front()->GetComponent<Text>()->text = L"조경";
	buttons_BuildingType[3]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::landscape); });
	buttons_BuildingType[4]->children.front()->GetComponent<Text>()->text = L"소품";
	buttons_BuildingType[4]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Decoration); });
	buttons_BuildingType[5]->children.front()->GetComponent<Text>()->text = L"삭제";
	buttons_BuildingType[5]->AddComponent<Button>()->AddEvent( [](void*) { ButtonManager::buttonManager->SelectButton(ButtonType::Delete); });

	for (int i = 0; i < 5; ++i)
	{
		butttons_BuildingList[i].push_back(CreateButtonList());
		ButtonManager::buttonManager->buttons_BuildingList[i].push_back(butttons_BuildingList[i][0]);
	}
	butttons_BuildingList[0][0]->children[0]->children[0]->GetComponent<Text>()->text = L"Well";
	butttons_BuildingList[0][0]->children[0]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::Well_01); });

	butttons_BuildingList[1][0]->children[0]->children[0]->GetComponent<Text>()->text = L"House01";
	butttons_BuildingList[1][0]->children[0]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::House_01, Scene::scene->geometries["SM_House_Var01"].get(), 3, 0.02f, 5.5f); });
	butttons_BuildingList[1][0]->children[1]->children[0]->GetComponent<Text>()->text = L"House02";
	butttons_BuildingList[1][0]->children[1]->AddComponent<Button>()->AddEvent([](void*) { BuildManager::buildManager->SelectBuilding(BuildingType::House_02); });

	//SM_Tree_Var04
	butttons_BuildingList[3].push_back(CreateButtonList());
	ButtonManager::buttonManager->buttons_BuildingList[3].push_back(butttons_BuildingList[3][1]);
	butttons_BuildingList[3][0]->children[0]->children[0]->GetComponent<Text>()->text = L"Tree_01";
	butttons_BuildingList[3][0]->children[0]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Tree_01, Scene::scene->geometries["SM_Tree_Var01"].get(), 7, 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[1]->children[0]->GetComponent<Text>()->text = L"Tree_02";
	butttons_BuildingList[3][0]->children[1]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Tree_02, Scene::scene->geometries["SM_Tree_Var02"].get(), 7, 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[2]->children[0]->GetComponent<Text>()->text = L"Tree_03";
	butttons_BuildingList[3][0]->children[2]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Tree_03, Scene::scene->geometries["SM_Tree_Var03"].get(), 7, 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[3]->children[0]->GetComponent<Text>()->text = L"Tree_04";
	butttons_BuildingList[3][0]->children[3]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Tree_04, Scene::scene->geometries["SM_Tree_Var04"].get(), 7, 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[4]->children[0]->GetComponent<Text>()->text = L"Tree_05";
	butttons_BuildingList[3][0]->children[4]->AddComponent<Button>()->AddEvent([](void*) {
		BuildManager::buildManager->SelectBuilding(BuildingType::Tree_05, Scene::scene->geometries["SM_Tree_Var05"].get(), 7, 0.02f, 5.0f); });
	butttons_BuildingList[3][0]->children[5]->children[0]->GetComponent<Text>()->text = L"Flower_01";
	butttons_BuildingList[3][0]->children[6]->children[0]->GetComponent<Text>()->text = L"Flower_02";
	butttons_BuildingList[3][0]->children[7]->children[0]->GetComponent<Text>()->text = L"Flower_03";
	butttons_BuildingList[3][0]->children[8]->children[0]->GetComponent<Text>()->text = L"Flower_04";
	butttons_BuildingList[3][0]->children[9]->children[0]->GetComponent<Text>()->text = L"Flower_05";
	butttons_BuildingList[3][1]->children[0]->children[0]->GetComponent<Text>()->text = L"Stone_Big_01";
	butttons_BuildingList[3][1]->children[1]->children[0]->GetComponent<Text>()->text = L"Stone_Big_02";
	butttons_BuildingList[3][1]->children[2]->children[0]->GetComponent<Text>()->text = L"Stone_Big_03";
	butttons_BuildingList[3][1]->children[3]->children[0]->GetComponent<Text>()->text = L"Stone_Big_04";
	butttons_BuildingList[3][1]->children[4]->children[0]->GetComponent<Text>()->text = L"Stone_Big_05";
	butttons_BuildingList[3][1]->children[5]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_01";
	butttons_BuildingList[3][1]->children[6]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_02";
	butttons_BuildingList[3][1]->children[7]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_03";
	butttons_BuildingList[3][1]->children[8]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_04";
	butttons_BuildingList[3][1]->children[9]->children[0]->GetComponent<Text>()->text = L"Stone_Medium_05";

	butttons_BuildingList[4].push_back(CreateButtonList());
	ButtonManager::buttonManager->buttons_BuildingList[4].push_back(butttons_BuildingList[4][1]);
	butttons_BuildingList[4][0]->children[0]->children[0]->GetComponent<Text>()->text = L"Fence_01";
	butttons_BuildingList[4][0]->children[1]->children[0]->GetComponent<Text>()->text = L"Fence_02";
	butttons_BuildingList[4][0]->children[2]->children[0]->GetComponent<Text>()->text = L"Fence_03";
	butttons_BuildingList[4][0]->children[3]->children[0]->GetComponent<Text>()->text = L"Fence_04";
	butttons_BuildingList[4][0]->children[4]->children[0]->GetComponent<Text>()->text = L"Bucket";
	butttons_BuildingList[4][0]->children[5]->children[0]->GetComponent<Text>()->text = L"Barrel";
	butttons_BuildingList[4][0]->children[6]->children[0]->GetComponent<Text>()->text = L"Pitchfork";
	butttons_BuildingList[4][0]->children[7]->children[0]->GetComponent<Text>()->text = L"Axe";
	butttons_BuildingList[4][0]->children[8]->children[0]->GetComponent<Text>()->text = L"Ladder";
	butttons_BuildingList[4][0]->children[9]->children[0]->GetComponent<Text>()->text = L"Spike";
	butttons_BuildingList[4][1]->children[0]->children[0]->GetComponent<Text>()->text = L"Cart_01";
	butttons_BuildingList[4][1]->children[1]->children[0]->GetComponent<Text>()->text = L"Cart_02";
	butttons_BuildingList[4][1]->children[2]->children[0]->GetComponent<Text>()->text = L"Torch_01";
	butttons_BuildingList[4][1]->children[3]->children[0]->GetComponent<Text>()->text = L"Torch_02";
	butttons_BuildingList[4][1]->children[4]->children[0]->GetComponent<Text>()->text = L"Logs_01";
	butttons_BuildingList[4][1]->children[5]->children[0]->GetComponent<Text>()->text = L"Logs_02";
	butttons_BuildingList[4][1]->children[6]->children[0]->GetComponent<Text>()->text = L"Log_01";
	butttons_BuildingList[4][1]->children[7]->children[0]->GetComponent<Text>()->text = L"Log_02";
	butttons_BuildingList[4][1]->children[8]->children[0]->GetComponent<Text>()->text = L"Log_03";
	butttons_BuildingList[4][1]->children[9]->children[0]->GetComponent<Text>()->text = L"Log_04";

	GameObject* button_previouspage = CreateImage();
	{
		auto rectTransform = button_previouspage->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = 10;
		rectTransform->posY = 60;
		rectTransform->width = 20;
		rectTransform->height = 20;

		button_previouspage->AddComponent<Button>()->AddEvent([](void*) { ButtonManager::buttonManager->PreviousPage(); });
		button_previouspage->SetActive(false);
		ButtonManager::buttonManager->buttons_page[0] = button_previouspage;
	}
	GameObject* button_nextpage = CreateImage();
	{
		auto rectTransform = button_nextpage->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = CyanFW::Instance()->GetWidth() - 30;
		rectTransform->posY = 60;
		rectTransform->width = 20;
		rectTransform->height = 20;

		button_nextpage->AddComponent<Button>()->AddEvent([](void*) { ButtonManager::buttonManager->NextPage(); });
		button_nextpage->SetActive(false);
		ButtonManager::buttonManager->buttons_page[1] = button_nextpage;
	}

}