#include "pch.h"
#include "TerrainScene.h"

BuildManager* BuildManager::buildManager{ nullptr };

void TerrainScene::BuildObjects()
{
	///*** Asset ***///
	//*** Texture ***//
	{
		AddTexture(0, "none", L"Textures\\none.dds");
		AddTexture(1, "ground", L"Textures\\grass.dds");
		AddTexture(2, "bricksTex", L"Textures\\bricks2.dds");
		AddTexture(3, "stoneTex", L"Textures\\stone.dds");
		AddTexture(4, "tileTex", L"Textures\\tile.dds");
		AddTexture(6, "tree", L"Textures\\tree01S.dds");
		AddTexture(7, "grass", L"Textures\\grass01.dds");
	}


	//*** Material ***//
	{
		AddMaterial(0, "none", 0);
		AddMaterial(1, "ground", 1, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		AddMaterial(2, "bricksMat", 2, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.02f, 0.02f, 0.02f }, 0.1f);
		AddMaterial(3, "stoneMat", 0, -1, { 0.0f, 0.0f, 0.1f, 1.0f }, { 0.98f, 0.97f, 0.95f }, 0.1f);
		AddMaterial(4, "tile0", 4, -1, { 0.9f, 0.9f, 0.9f, 1.0f }, { 0.02f, 0.02f, 0.02f }, 0.1f, Matrix4x4::MatrixScaling(8, 8, 1));
		AddMaterial(5, "sky", 5, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 1.0f);
		AddMaterial(6, "tree0", 6, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
		AddMaterial(7, "grass", 7, -1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
		for (int i = 0; i < 5; ++i)
			AddMaterial(8 + i, "material_" + std::to_string(i), 0, 0, RANDOM_COLOR, { 0.98f, 0.97f, 0.95f }, 0.0f);
	}

	//*** Mesh ***//
	{
		geometries["Image"] = Mesh::CreateQuad();
		geometries["Cube"] = Mesh::CreateCube();
		geometries["Plane"] = Mesh::CreatePlane();
		geometries["Sphere"] = Mesh::CreateSphere();
		geometries["Cylinder"] = Mesh::CreateCylinder();
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

		renderObjectsLayer[(int)RenderLayer::Sky][mesh].gameObjects.push_back(ritem);
	}


	GameObject* grid = CreateEmpty();
	{
		grid->GetComponent<Transform>()->position -= {128, 10, 128};
		auto mesh = grid->AddComponent<MeshFilter>()->mesh = gridMesh;
		grid->AddComponent<Renderer>()->materials.push_back(1);
		renderObjectsLayer[(int)RenderLayer::Opaque][mesh].gameObjects.push_back(grid);
	}

	{
		GameObject* buildManager = CreateEmpty();
		BuildManager* bm = buildManager->AddComponent<BuildManager>();
		bm->terrain = grid;
		bm->heightMap = m_pHeightMapImage;
		bm->terrainMesh = gridMesh;
		//bm->SelectModel(geometries["Cube"].get(), 8, 5);
		BuildManager::buildManager = bm;
	}

	// billboard points
	struct TreeSpriteVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Size;
		XMFLOAT3 look;
	};
	std::vector<TreeSpriteVertex> vertices;
	float sizex = 2, sizey = 2;
	const int width = 256, length = 256;
	vertices.reserve(width * length);
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < length; ++j)
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

	{
		GameObject* billboards = CreateEmpty();
		billboards->GetComponent<Transform>()->position -= {128, 10, 128};
		auto mesh = billboards->AddComponent<MeshFilter>()->mesh = geometries["Grass"].get();
		billboards->AddComponent<Renderer>()->materials.push_back(7);
		renderObjectsLayer[(int)RenderLayer::Grass][mesh].gameObjects.push_back(billboards);
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
	auto BuildingSelectButton01 = CreateImage();
	{
		auto rectTransform = BuildingSelectButton01->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = 10;
		rectTransform->posY = 10;
		rectTransform->width = 50;
		rectTransform->height = 50;

		BuildingSelectButton01->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["Sphere"].get(), 9, 1);
			});
		{
			auto textobject = BuildingSelectButton01->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };

			Text* text = textobject->AddComponent<Text>();
			text->text = L"Sphere";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}

	auto BuildingSelectButton02 = CreateImage();
	{
		auto rectTransform = BuildingSelectButton02->GetComponent<RectTransform>();
		rectTransform->anchorMin = { 0, 0 };
		rectTransform->anchorMax = { 0, 0 };
		rectTransform->pivot = { 0, 0 };
		rectTransform->posX = 70;
		rectTransform->posY = 10;
		rectTransform->width = 50;
		rectTransform->height = 50;
	
		BuildingSelectButton02->AddComponent<Button>()->AddEvent(
			[](void*) {
				BuildManager::buildManager->SelectModel(Scene::scene->geometries["Cube"].get(), 8, 5);
			});
		{
			auto textobject = BuildingSelectButton02->AddChildUI();
			auto rectTransform = textobject->GetComponent<RectTransform>();
			rectTransform->anchorMin = { 0, 0 };
			rectTransform->anchorMax = { 1, 1 };
	
			Text* text = textobject->AddComponent<Text>();
			text->text = L"Cube";
			text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			textObjects.push_back(textobject);
		}
	}
}