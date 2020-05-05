#pragma once
#include "..\CyanEngine\framework.h"
#include "Building.h"
#include "RotatingBehavior.h"

enum class BuildingType
{
	/*LandMark*/
	Well_01 = 0,

	/*House*/
	House_01 = 100, House_02,

	/*Theme*/

	/*landscape*/
	Tree_01 = 300, Tree_02, Tree_03, Tree_04, Tree_05,
	Tree_Stump_01, Tree_Stump_02,
	Plant_01, Flower_01, Flower_02, Flower_03, Flower_04, Flower_05, Flower_06,
	Grass_01, Grass_02, Grass_03,
	Mushroom_01, Mushroom_02, Mushroom_03, Mushroom_04, Mushroom_05, Mushroom_06,
	Stone_Big_01, Stone_Big_02, Stone_Big_03, Stone_Big_04, Stone_Big_05, Stone_Big_06,
	Stone_Medium_01, Stone_Medium_02, Stone_Medium_03, Stone_Medium_04, Stone_Medium_05,
	Stone_Small_01, Stone_Small_02, Stone_Small_03, Stone_Small_04, Stone_Small_05,
	Stone_Flat_01, Stone_Flat_02, Stone_Flat_03, Stone_Flat_04, Stone_Flat_05,
	StonePath_01, StonePath_02, StonePath_03,

	/*Decoration*/
	Fence_01 = 400, Fence_02, Fence_03, Fence_04,
	Street_Light_01,
	Bucket, Barrel, Pitchfork, Axe, Ladder, Spike, Cart_01, Cart_02,
	Torch_01, Torch_02,
	Logs_01, Logs_02, Log_01, Log_02, Log_03, Log_04,
	Pot_01, Pot_02, Pot_03, Pot_04, Pot_05, Pot_06, Cauldron, Crate_Open, Crate_Closed,
	Hay_Small_01, Hay_Small_02, Hay_Stack,
	Apple, Potato, Tomato, Fish, Watermellon,
	Sack_Apple, Sack_Flour, Sack_Potato, Sack_Tomato, Sack_01, Sack_02,
	Pumpkin_01, Pumpkin_02, Pumpkin_03
};

class BuildManager : public MonoBehavior<BuildManager>
{
private /*�� ������ private ������ �����ϼ���.*/:
	BuildingType prefabType;
public  /*�� ������ public ������ �����ϼ���.*/:
	GameObject* prefab{ nullptr };
	GameObject* terrain;
	CHeightMapImage* heightMap;
	CHeightMapGridMesh* terrainMesh;
	float dT;

	bool rotationToggle{ false };
	Vector3 lastMousePos;
	static BuildManager* buildManager;

private:
	friend class GameObject;
	friend class MonoBehavior<BuildManager>;
	BuildManager() = default;
	BuildManager(BuildManager&) = default;

public:
	~BuildManager() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		if (prefab)
			Pick(Input::mousePosition.x, Input::mousePosition.y);
	}

	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.

	void Pick(int sx, int sy)
	{
		Vector3 screenPos{ (float)sx, (float)sy, 1.0f };

		Vector3 rayOrigin{ 0.0f, 0.0f, 0.0f };
		Vector3 rayDir = Camera::main->ScreenToWorldPoint(screenPos);

		Matrix4x4 invView = Camera::main->view.Inverse();
		Matrix4x4 invWorld = terrain->transform->localToWorldMatrix.Inverse();
		Matrix4x4 toLocal = invView * invWorld;

		rayOrigin = rayOrigin.TransformCoord(toLocal);
		rayDir = rayDir.TransformNormal(invWorld);

		rayDir = rayDir.Normalized();

		if (IntersectPlane(rayOrigin.xmf3, rayDir.xmf3, XMFLOAT3{ 0,0,0 }, XMFLOAT3{ 1,0,0 }, XMFLOAT3{ 0,0,1 }))
		{
			std::vector<XMFLOAT3> vertices;
			Vector3 point = rayOrigin + rayDir * dT;

			for (float i = 1; i < dT + 1; i += 0.5)
			{
				Vector3 p = rayOrigin + rayDir * i;
				if ((int)point.x == (int)p.x && (int)point.z == (int)p.z)
					continue;

				point = p;
				float x, y, z;
				if (std::ceil(point.x) > 256 || std::ceil(point.z) > 256)
					continue;

				x = std::floor(point.x);
				z = std::floor(point.z);
				vertices.push_back({ x,terrainMesh->OnGetHeight(x, z, heightMap),z });
				vertices.push_back({ x + 1,terrainMesh->OnGetHeight(x + 1, z + 1, heightMap),z + 1 });
				vertices.push_back({ x + 1,terrainMesh->OnGetHeight(x + 1, z, heightMap),z });

				vertices.push_back({ x,terrainMesh->OnGetHeight(x, z, heightMap),z });
				vertices.push_back({ x,terrainMesh->OnGetHeight(x, z + 1, heightMap),z + 1 });
				vertices.push_back({ x + 1,terrainMesh->OnGetHeight(x + 1, z + 1, heightMap),z + 1 });

			}
			IntersectVertices(rayOrigin.xmf3, rayDir.xmf3, vertices);
			point = rayOrigin + rayDir * dT;
			point = point.TransformCoord(terrain->transform->localToWorldMatrix);
			if (Input::GetKeyDown(KeyCode::T))
				rotationToggle = rotationToggle ? false : true;
			if (Input::GetMouseButtonDown(2))
				lastMousePos = Input::mousePosition;
			else if (Input::GetMouseButton(2))
			{
				if (rotationToggle)
				{
					if (abs(lastMousePos.y - Input::mousePosition.y) > 30)
					{
						prefab->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, (lastMousePos.y - Input::mousePosition.y) / abs(lastMousePos.y - Input::mousePosition.y) * 30.0f);
						lastMousePos = Input::mousePosition;
					}
				}
				else
				{
					prefab->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, (lastMousePos.y - Input::mousePosition.y) * Time::deltaTime * 60.0f);
					lastMousePos = Input::mousePosition;
				}

			}
			else if (Input::GetMouseButtonUp(0) && prefab->children.front()->GetComponent<Constant>()->v4.g == 1.0f)
			{
				prefab->children.front()->layer = (int)RenderLayer::Opaque;
				GameObject* go = Scene::scene->Duplicate(prefab);
				DeletePrefab();
			}
			else
				prefab->transform->position = { point.x, point.y, point.z };
		}
	}

	bool IntersectPlane(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2)
	{
		XMFLOAT3 edge1{ v1.x - v0.x,v1.y - v0.y,v1.z - v0.z };
		XMFLOAT3 edge2{ v2.x - v0.x,v2.y - v0.y, v2.z - v0.z };

		XMFLOAT3 pvec = NS_Vector3::CrossProduct(rayDirection, edge2);

		float dot = NS_Vector3::DotProduct(edge1, pvec);

		if (dot > 0.0001f)
			return false;

		XMFLOAT3 planeNormal = NS_Vector3::CrossProduct(edge1, edge2);

		float dot1 = NS_Vector3::DotProduct(planeNormal, v0);
		float dot2 = NS_Vector3::DotProduct(planeNormal, rayOrigin);
		float dot3 = NS_Vector3::DotProduct(planeNormal, rayDirection);

		dT = (dot1 - dot2) / dot3;

		return true;

	}

	void IntersectVertices(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, std::vector<XMFLOAT3>& vertices)
	{
		UINT triCount = vertices.size() / 3;

		float tmin = MathHelper::Infinity;
		for (UINT i = 0; i < triCount; ++i)
		{
			XMVECTOR v0 = XMLoadFloat3(&vertices[i * 3 + 0]);
			XMVECTOR v1 = XMLoadFloat3(&vertices[i * 3 + 1]);
			XMVECTOR v2 = XMLoadFloat3(&vertices[i * 3 + 2]);

			float x = (vertices[i * 3 + 0].x + vertices[i * 3 + 1].x + vertices[i * 3 + 2].x) / 3;
			float y = (vertices[i * 3 + 0].y + vertices[i * 3 + 1].y + vertices[i * 3 + 2].y) / 3;
			float z = (vertices[i * 3 + 0].z + vertices[i * 3 + 1].z + vertices[i * 3 + 2].z) / 3;
			XMMATRIX toLocal = XMMatrixTranslation(-x, -y, -z);
			XMMATRIX InvToLocal = XMMatrixInverse(&XMMatrixDeterminant(toLocal), toLocal);
			XMMATRIX s = XMMatrixScaling(1.2, 1.2, 1.2);

			v0 = XMVector3Transform(XMVector3Transform(XMVector3Transform(v0, toLocal), s), InvToLocal);
			v1 = XMVector3Transform(XMVector3Transform(XMVector3Transform(v1, toLocal), s), InvToLocal);
			v2 = XMVector3Transform(XMVector3Transform(XMVector3Transform(v2, toLocal), s), InvToLocal);

			float t = 0.0f;
			if (TriangleTests::Intersects(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), v0, v1, v2, t))
			{
				if (t < tmin)
				{
					tmin = t;
					dT = tmin;
				}
			}
		}
	}

	void SelectBuilding(BuildingType type, Mesh* mesh = NULL, Material* mat = NULL, float scaleSize = 0.02f, float colliderSize = NULL)
	{
		if (prefab) {
			DeletePrefab();
			if (prefabType == type) return;
		}

		switch (type)
		{
		case BuildingType::Well_01:
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			prefab->AddComponent<BoxCollider>()->extents = { 3.0f, 3.0f, 3.0f };
			{
				GameObject* child = prefab->AddChild();
				child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_Well");
				child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("material_03"));
				child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
				child->layer = (int)RenderLayer::BuildPreview;
				child->transform->Scale({ 0.02f, 0.02f, 0.02f });
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				GameObject* child = prefab->AddChild();
				child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_Well_Extra02");
				child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("material_03"));
				child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
				child->layer = (int)RenderLayer::BuildPreview;
				child->transform->position = { 0.0f,3.0f,0.0f };
				child->transform->Scale({ 0.02f, 0.02f, 0.02f });
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				GameObject* child = prefab->AddChild();
				child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_Well_Extra03");
				child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("material_03"));
				child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
				child->layer = (int)RenderLayer::BuildPreview;
				child->transform->position = { 0.0f,2.0f,0.0f };
				child->transform->Scale({ 0.02f, 0.02f, 0.02f });
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			break;
		case BuildingType::House_02:
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			prefab->AddComponent<BoxCollider>()->extents = { 5.5f, 5.5f, 5.5f };
			{
				GameObject* child = prefab->AddChild();
				child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var02");
				child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house02"));
				child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
				child->layer = (int)RenderLayer::BuildPreview;
				child->transform->Scale({ 0.02f, 0.02f, 0.02f });
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				GameObject* child = prefab->AddChild();
				child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var02_Extra");
				child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house02"));
				child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
				child->AddComponent<RotatingBehavior>()->speedRotating = -10.0f;
				child->layer = (int)RenderLayer::BuildPreview;
				child->transform->position = { 0.5f,12.0f,5.0f };
				child->transform->Scale({ 0.02f, 0.02f, 0.02f });
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			break;
		case BuildingType::Fence_01:
			mesh = ASSET MESH("SM_Fence_Var01");
		case BuildingType::Fence_02:
			if (mesh == NULL) mesh = ASSET MESH("SM_Fence_Var02");
			mat = ASSET MATERIAL("material_01");
		{
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			BoxCollider* collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 3.2f, 1.0f, 0.8f };
			collider->obb = true;

			GameObject* child = prefab->AddChild();
			child->AddComponent<MeshFilter>()->mesh = mesh;
			Renderer* renderer = child->AddComponent<Renderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(mat);
			child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			child->layer = (int)RenderLayer::BuildPreview;
			child->transform->Scale({ scaleSize, scaleSize, scaleSize });
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
		}	
			break;
		case BuildingType::Fence_03:
			mesh = ASSET MESH("SM_Fence_Var03");
		case BuildingType::Fence_04:
			if (mesh == NULL) mesh = ASSET MESH("SM_Fence_Var04");
			mat = ASSET MATERIAL("material_01");
		{
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			BoxCollider* collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 3.0f, 1.0f, 0.8f };
			collider->obb = true;

			GameObject* child = prefab->AddChild();
			child->AddComponent<MeshFilter>()->mesh = mesh;
			Renderer* renderer = child->AddComponent<Renderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(mat);
			child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			child->layer = (int)RenderLayer::BuildPreview;
			child->transform->Scale({ scaleSize, scaleSize, scaleSize });
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
		}
		break;
		case BuildingType::Spike:
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
		{
			BoxCollider* collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 2.8f, 1.0f, 0.8f };
			collider->obb = true;
			mesh = ASSET MESH("SM_Spike");
			mat = ASSET MATERIAL("material_02");
			GameObject* child = prefab->AddChild();
			child->AddComponent<MeshFilter>()->mesh = mesh;
			Renderer* renderer = child->AddComponent<Renderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(mat);
			child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			child->layer = (int)RenderLayer::BuildPreview;
			child->transform->Scale({ scaleSize, scaleSize, scaleSize });
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
		}
		{
			mesh = ASSET MESH("SM_Fish");
			GameObject* child = prefab->AddChild();
			child->AddComponent<MeshFilter>()->mesh = mesh;
			Renderer* renderer = child->AddComponent<Renderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(mat);
			child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			child->layer = (int)RenderLayer::BuildPreview;
			child->transform->position = { -0.2f,1.5f,0.0f };
			child->transform->Scale({ scaleSize, scaleSize, scaleSize });
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
		}
			break;
		case BuildingType::Cart_01:
			mesh = ASSET MESH("SM_Cart_Var01");
		case BuildingType::Cart_02:
			mesh = ASSET MESH("SM_Cart_Var02");
			mat = ASSET MATERIAL("material_01");
		{
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			BoxCollider* collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 4.0f, 3.0f, 2.2f };
			collider->obb = true;

			GameObject* child = prefab->AddChild();
			child->AddComponent<MeshFilter>()->mesh = mesh;
			Renderer* renderer = child->AddComponent<Renderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(mat);
			child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			child->layer = (int)RenderLayer::BuildPreview;
			child->transform->Scale({ scaleSize, scaleSize, scaleSize });
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
		}
		break;
		default:
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			prefab->AddComponent<BoxCollider>()->extents = { colliderSize, colliderSize, colliderSize };

			GameObject* child = prefab->AddChild();
			child->AddComponent<MeshFilter>()->mesh = mesh;
			Renderer* renderer = child->AddComponent<Renderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(mat);
			child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			child->layer = (int)RenderLayer::BuildPreview;
			child->transform->Scale({ scaleSize, scaleSize, scaleSize });
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			break;
		}
	
		prefabType = type;
	}

	void DeletePrefab()
	{
		Scene::scene->PushDelete(prefab);
		prefab = nullptr;
	}
};