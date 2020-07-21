#pragma once
#include "Scene.h"

class GuestScene: public Scene
{
public:
	GuestScene() : Scene() {}
	~GuestScene() {}

	virtual void BuildObjects();

	void LoadTextureAsset()
	{
		//ASSET AddTexture("none", L"Textures\\none.dds");
		ASSET AddTexture("ground", L"Textures\\grass.dds");
		ASSET AddTexture("grass", L"Texture\\grass.dds");
		ASSET AddTexture("house01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_04_D.dds");
		ASSET AddTexture("house02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_09_D.dds");
		ASSET AddTexture("material_01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_01_D.dds");
		ASSET AddTexture("material_02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_02_D.dds");
		ASSET AddTexture("material_03", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_03_D.dds");
		ASSET AddTexture("TreeLeafs", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeLeafs_D.dds");
		ASSET AddTexture("TreeTrunks", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeTrunks_D.dds");
		ASSET AddTexture("polyArtTex", L"Textures\\PolyArtTex.dds");
		ASSET AddTexture("fireTexD", L"Textures\\fire.dds");
		ASSET AddTexture("smokeTexD", L"Textures\\smoke.dds");
	}

	void LoadMaterialAsset()
	{
		//ASSET AddMaterial("none", ASSET TEXTURE("none"));
		//ASSET AddMaterial("yellow", ASSET TEXTURE("none"), -1, { 0.8f, 0.7f, 0.1f, 1.0f });
		ASSET AddMaterial("ground", ASSET TEXTURE("ground"), nullptr, { 0.48f, 0.64f, 0.2f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f, Matrix4x4::MatrixScaling(200, 200, 200));
		ASSET AddMaterial("grass", ASSET TEXTURE("grass"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);
		ASSET AddMaterial("house01", ASSET TEXTURE("house01"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("house02", ASSET TEXTURE("house02"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_01", ASSET TEXTURE("material_01"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_02", ASSET TEXTURE("material_02"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_03", ASSET TEXTURE("material_03"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("none", ASSET TEXTURE("none"));
		ASSET AddMaterial("gray", ASSET TEXTURE("none"), nullptr, { 0.5, 0.5, 0.5, 1.0 });
		ASSET AddMaterial("PolyArt", ASSET TEXTURE("polyArtTex"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("fireMat", ASSET TEXTURE("fireTexD"));
		ASSET AddMaterial("smokeMat", ASSET TEXTURE("smokeTexD"));
		ASSET AddMaterial("TreeLeafs", ASSET TEXTURE("TreeLeafs"));
		ASSET AddMaterial("TreeTrunks", ASSET TEXTURE("TreeTrunks"));
	}

	void LoadMeshAsset()
	{
		//ASSET AddMesh("Image", Mesh::CreateQuad());
		//ASSET AddMesh("Sphere", Mesh::CreateSphere());
		ASSET AddFbxForMesh("SM_Well", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well.FBX");
		ASSET AddFbxForMesh("SM_Well_Extra02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra02.FBX");
		ASSET AddFbxForMesh("SM_Well_Extra03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra03.FBX");
		ASSET AddFbxForMesh("SM_House_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
		ASSET AddFbxForMesh("SM_House_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02.FBX");
		ASSET AddFbxForMesh("SM_House_Var02_Extra", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02_Extra.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var01.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var02.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var03.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var04.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var05.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var01.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var02.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var03.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var04.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var05.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var06.FBX");
		ASSET AddFbxForMesh("SM_Cart_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var01.FBX");
		ASSET AddFbxForMesh("SM_Cart_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var02.FBX");
		ASSET AddFbxForMesh("SM_Barrel", "Assets\\AdvancedVillagePack\\Meshes\\SM_Barrel.FBX");
		ASSET AddFbxForMesh("SM_Bucket", "Assets\\AdvancedVillagePack\\Meshes\\SM_Bucket.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var01.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var02.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var03.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var04.FBX");
		ASSET AddFbxForMesh("SM_Cauldron", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cauldron.FBX");
		ASSET AddFbxForMesh("SM_Spike", "Assets\\AdvancedVillagePack\\Meshes\\SM_Spike.FBX");
		ASSET AddFbxForMesh("SM_Fish", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fish.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var01.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var02.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var03.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var04.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var05.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var06.FBX");
		ASSET AddFbxForMesh("SM_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Apple.FBX");
		ASSET AddFbxForMesh("SM_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Potato.FBX");
		ASSET AddFbxForMesh("SM_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tomato.FBX");
		ASSET AddFbxForMesh("SM_Watermellon", "Assets\\AdvancedVillagePack\\Meshes\\SM_Watermellon.FBX");
		ASSET AddFbxForMesh("SM_Sack_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Apple.FBX");
		ASSET AddFbxForMesh("SM_Sack_Flour", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Flour.FBX");
		ASSET AddFbxForMesh("SM_Sack_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Potato.FBX");
		ASSET AddFbxForMesh("SM_Sack_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Tomato.FBX");
		ASSET AddFbxForMesh("SM_Sack_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var01.FBX");
		ASSET AddFbxForMesh("SM_Sack_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var02.FBX");

		ASSET AddFbxForAnimation("ApprenticeSK", "Models\\modelTest.fbx");

		//*** Animation ***//
		ASSET AddFbxForAnimation("Walk_BowAnim", "Models\\BowStance\\Walk_BowAnim.fbx");
		ASSET AddFbxForAnimation("WalkBack_BowAnim", "Models\\BowStance\\WalkBack_BowAnim.fbx");
		ASSET AddFbxForAnimation("WalkRight_BowAnim", "Models\\BowStance\\WalkRight_BowAnim.fbx");
		ASSET AddFbxForAnimation("WalkLeft_BowAnim", "Models\\BowStance\\WalkLeft_BowAnim.fbx");
		ASSET AddFbxForAnimation("Idle_BowAnim", "Models\\BowStance\\Idle_BowAnim.fbx");
	}

	void LoadPrefab()
	{
		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			collider->obb = true;
			Vector3 boundMin{ FLT_MAX }, boundMax{ -FLT_MAX }, center, extents;
			BoundingBox bb0, bb1, bb2;
			{
				auto child = MakePrefab(ASSET MESH("SM_Well"), ASSET MATERIAL("material_03"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb0 = ASSET MESH("SM_Well")->Bounds;
				center.xmf3 = bb0.Center; extents.xmf3 = bb0.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			{
				auto child = MakePrefab(ASSET MESH("SM_Well_Extra02"), ASSET MATERIAL("material_03"), prefab);
				child->transform->position = { 0.0f,1.5f,0.0f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb1 = ASSET MESH("SM_Well_Extra02")->Bounds;
				center.xmf3 = bb1.Center; extents.xmf3 = bb1.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			{
				auto child = MakePrefab(ASSET MESH("SM_Well_Extra03"), ASSET MATERIAL("material_03"), prefab);
				child->transform->position = { 0.0f,1.0f,0.0f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb2 = ASSET MESH("SM_Well_Extra03")->Bounds;
				center.xmf3 = bb2.Center; extents.xmf3 = bb2.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			collider->center = (boundMax + boundMin) / 2;
			collider->extents = (boundMax - boundMin) / 2;

			ASSET AddPrefab("Well", std::make_unique<GameObject>(prefab));
		}

		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 2.5f, 4.5f, 2.5f };
			collider->center.y += collider->extents.y * 0.5f;
			collider->obb = true;
			{
				auto child = MakePrefab(ASSET MESH("SM_House_Var02"), ASSET MATERIAL("house02"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				auto child = MakePrefab(ASSET MESH("SM_House_Var02_Extra"), ASSET MATERIAL("house02"), prefab);
				child->AddComponent<RotatingBehavior>()->speedRotating = -10.0f;
				child->transform->position = { 0.25f,6.0f,2.5f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			ASSET AddPrefab("House01", std::make_unique<GameObject>(prefab));
		}

		{
			auto prefab = MakePrefab();
			auto collider = prefab->AddComponent<BoxCollider>()->obb;

			ASSET AddPrefab("MRC", std::make_unique<GameObject>(prefab));
		}
	}

	GameObject* MakePrefab(Mesh* mesh = nullptr, Material* material = nullptr, GameObject* parent = nullptr)
	{
		auto prefab = parent ? parent->AddChild() : CreateEmptyPrefab();
		prefab->AddComponent<MeshFilter>()->mesh = mesh;
		auto renderer = prefab->AddComponent<Renderer>();
		if (material)
			renderer->materials.push_back(material);
		return prefab;
	}
};