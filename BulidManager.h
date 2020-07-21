#pragma once
#include "..\CyanEngine\framework.h"
#include "Building.h"
#include "RotatingBehavior.h"
#include "HostNetwork.h"
#include "ParticleManager.h"

class BuildManager : public MonoBehavior<BuildManager>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	BuildingType prefabType;
public  /*이 영역에 public 변수를 선언하세요.*/:
	GameObject* prefab{ nullptr };
	GameObject* terrain;
	TerrainData* heightMap;
	RenderTexture* terrainMesh;
	float distance;

	std::vector<ParticleManager*> particles;

	bool deleteButtonState{ false };
	bool rotationToggle{ false };
	Vector3 lastMousePos;

	unordered_map<BuildingInform, GameObject*, BuildingInformHasher> buildings;
	static BuildManager* buildManager;
private:
	friend class GameObject;
	friend class MonoBehavior<BuildManager>;
	BuildManager() = default;
	BuildManager(BuildManager&) = default;

public:
	~BuildManager() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		if (prefab)
			Pick(Input::mousePosition.x, Input::mousePosition.y);
		if (deleteButtonState)
			PickObject(Input::mousePosition.x, Input::mousePosition.y);
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.

	void Pick(int mx, int my)
	{
		Vector3 screenPos{ (float)mx, (float)my, 1.0f };

		Vector3 rayOrigin{ 0.0f, 0.0f, 0.0f };
		Vector3 rayDir = Camera::main->ScreenToWorldPoint(screenPos);

		Matrix4x4 invView = Camera::main->view.Inverse();
		Matrix4x4 invWorld = terrain->transform->localToWorldMatrix.Inverse();
		Matrix4x4 toLocal = invView * invWorld;

		rayOrigin = rayOrigin.TransformCoord(toLocal);
		rayDir = rayDir.TransformNormal(invWorld).Normalize();

		if (IntersectPlane(rayOrigin, rayDir, Vector3{ 0,0,0 }, Vector3{ 1,0,0 }, Vector3{ 0,0,1 }))
		{
			Vector3 point;
			if (heightMap)
			{
				std::vector<XMFLOAT3> vertices;
				point = rayOrigin + rayDir * distance;

				for (float i = 1; i < distance + 1; i += 0.5)
				{
					Vector3 p = rayOrigin + rayDir * i;
					if ((int)point.x == (int)p.x && (int)point.z == (int)p.z)
						continue;

					point = p;
					float x, y, z;
					if (std::ceil(point.x) > heightMap->GetHeightMapWidth() - 1 || std::ceil(point.z) > heightMap->GetHeightMapLength() - 1)
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
			}

			point = rayOrigin + rayDir * distance;
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
			else if (Input::GetMouseButtonUp(0))// && prefab->children.front()->GetComponent<Constant>()->v4.g == 1.0f)
			{
				CreateBuilding();
			}
			else
				prefab->transform->position = { point.x, point.y, point.z };
		}
	}

	void PickObject(int mx, int my)
	{
		Vector3 screenPos{ (float)mx, (float)my, 1.0f };

		Vector3 rayOrigin{ 0.0f, 0.0f, 0.0f };
		Vector3 rayDir = Camera::main->ScreenToWorldPoint(screenPos).Normalize();

		rayOrigin = rayOrigin.TransformCoord(Camera::main->view.Inverse());

		GameObject* obj{ nullptr };
		float minDist = MathHelper::Infinity;
		for (auto& go : gameObject->scene->gameObjects)
		{
			BoxCollider* collider = go->GetComponent<BoxCollider>();
			if (collider)
			{
				BoundingBox b = collider->boundingBox;
				XMFLOAT4 quarternion;
				XMStoreFloat4(&quarternion, DirectX::XMQuaternionRotationMatrix(XMLoadFloat4x4(&go->transform->localToWorldMatrix.xmf4x4)));
				BoundingOrientedBox boundingBox;
				boundingBox.Orientation = quarternion;
				boundingBox.Center = b.Center;
				if (boundingBox.Intersects(XMLoadFloat3(&rayOrigin.xmf3), XMLoadFloat3(&rayDir.xmf3), distance))
					if (minDist > distance)
					{
						obj = go;
						minDist = distance;
					}
			}	
		}
		if (obj != nullptr)
		{
			if (Input::GetMouseButtonUp(0))
			{
				BuildingInform b_inform;				
				b_inform.xPos = obj->transform->position.x;
				b_inform.yPos = obj->transform->position.y;
				b_inform.zPos = obj->transform->position.z;
				auto it = buildings.find(b_inform);
				if (it != buildings.end())
				{
					if (HostNetwork::network != nullptr)
					{
						if (HostNetwork::network->isConnect)
						{	// Networking
							HostNetwork::network->send_destruct_packet(it->first);
						}
					}
					Scene::scene->PushDelete(it->second);
					BuildManager::buildManager->buildings.erase(it);
				}
			}
		}
	}

	bool IntersectPlane(Vector3 rayOrigin, Vector3 rayDirection, Vector3 v0, Vector3 v1, Vector3 v2)
	{
		Vector3 edge1{ v1.x - v0.x,v1.y - v0.y,v1.z - v0.z };
		Vector3 edge2{ v2.x - v0.x,v2.y - v0.y, v2.z - v0.z };

		Vector3 pvec = Vector3::CrossProduct(rayDirection, edge2);

		float dot = Vector3::DotProduct(edge1, pvec);

		if (dot > 0.0001f)
			return false;

		Vector3 planeNormal = Vector3::CrossProduct(edge1, edge2);

		float dot1 = Vector3::DotProduct(planeNormal, v0);
		float dot2 = Vector3::DotProduct(planeNormal, rayOrigin);
		float dot3 = Vector3::DotProduct(planeNormal, rayDirection);

		distance = (dot1 - dot2) / dot3;

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
					distance = tmin;
				}
			}
		}
	}

	void SelectBuilding(BuildingType type, Mesh* mesh = NULL, Material* mat = NULL, float scaleSize = 0.01f, float colliderSize = NULL)
	{
		if (prefab) {
			DeletePrefab();
			if (prefabType == type) return;
		}

		scaleSize = { 1.0f };

		switch (type)
		{
		case BuildingType::Well_01:
			prefab = Scene::scene->Duplicate(ASSET PREFAB("Well"));
			break;
		case BuildingType::House_02:
			prefab = Scene::scene->Duplicate(ASSET PREFAB("House01"));
			break;
		case BuildingType::Fence_01:
			mesh = ASSET MESH("SM_Fence_Var01");
		case BuildingType::Fence_02:
			if (mesh == NULL)
				mesh = ASSET MESH("SM_Fence_Var02");
			mat = ASSET MATERIAL("material_01");
		{
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			BoxCollider* collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 1.6f, 0.5f, 0.4f };
			collider->center.y += collider->extents.y * 0.5f;
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
			collider->extents = { 1.5f, 0.5f, 0.4f };
			collider->center.y += collider->extents.y * 0.5f;
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
		{
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<Building>();
			{
				BoxCollider* collider = prefab->AddComponent<BoxCollider>();
				collider->extents = { 1.4f, 0.8f, 0.4f };
				collider->center.y += collider->extents.y * 0.5f;
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
				child->transform->position = { -0.1f,0.75f,0.0f };
				child->transform->Scale({ scaleSize, scaleSize, scaleSize });
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
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
			collider->extents = { 1.7f, 0.5f, 1.0f };
			collider->center.y += collider->extents.y * 0.5f;
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
		{
			prefab = Scene::scene->Duplicate(ASSET PREFAB("MRC"));
			auto collider = prefab->GetComponent<BoxCollider>();
			collider->extents = { colliderSize, colliderSize, colliderSize };
			collider->center.y += collider->extents.y * 0.5f;

			prefab->GetComponent<MeshFilter>()->mesh = mesh;
			auto renderer = prefab->GetComponent<Renderer>();
			for (auto& sm : mesh->DrawArgs)
				renderer->materials.push_back(mat);
			prefab->transform->Scale({ scaleSize, scaleSize, scaleSize });
			prefab->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
		}
			break;
		}
	
		prefabType = type;
	}

	void DeletePrefab()
	{
		Scene::scene->PushDelete(prefab);
		prefab = nullptr;
	}

	void CreateBuilding()
	{
		if (!prefab)
			return;
		if(prefab->children.size())
			prefab->children.front()->layer = (int)RenderLayer::Opaque;
		else
			prefab->layer = (int)RenderLayer::Opaque;
		prefab->AddComponent<Building>()->positionToAnimate = prefab->transform->position;
		GameObject* go = Scene::scene->Duplicate(prefab);		

		BuildingInform b_inform;
		b_inform.buildingType = prefabType;
		Vector3 prefabForward = Vector3::Normalize(prefab->transform->localToWorldMatrix.forward);
		Vector3 forward = { 0.0f, 0.0f, 1.0f };
		float angle = Vector3::DotProduct(forward, prefabForward);
		Vector3 dir = Vector3::CrossProduct(forward, prefabForward);
		b_inform.rotAngle = XMConvertToDegrees(acos(angle));
		b_inform.rotAngle *= (dir.y > 0.0f) ? 1.0f : -1.0f;
		b_inform.xPos = prefab->transform->position.x;
		b_inform.yPos = prefab->transform->position.y;
		b_inform.zPos = prefab->transform->position.z;

		buildings[b_inform] = go;
		if (HostNetwork::network->isConnect)
		{	// Networking
			HostNetwork::network->send_construct_packet(b_inform);
		}
		BoxCollider* collider = prefab->GetComponent<BoxCollider>();
		/*Create Effect*/
		SetParticle(collider->extents.y * 4, go->transform->position);
		go->transform->position.y -= collider->extents.y * 2 + 0.5f;

		if (!Input::GetKey(KeyCode::Shift))
			DeletePrefab();
	}

	void SetParticle(float time, Vector3 pos)
	{
		for (auto& particle : particles)
		{
			if (!particle->particleSystem->enabled)
			{
				particle->Enable(time, pos);
				return;
			}
		}
	}
};