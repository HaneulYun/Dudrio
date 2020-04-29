#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class BuildManager : public MonoBehavior<BuildManager>
{
private /*�� ������ private ������ �����ϼ���.*/:
	//Mesh* model;
	GameObject* prefab{ nullptr };

public  /*�� ������ public ������ �����ϼ���.*/:
	GameObject* terrain;
	CHeightMapImage* heightMap;
	CHeightMapGridMesh* terrainMesh;
	float dT;

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
			prefab->transform->position = { point.x, point.y + 1.0f, point.z };
			prefab->NumFramesDirty = NUM_FRAME_RESOURCES;
			if (Input::GetMouseButtonUp(2))
			{
				GameObject* go = Scene::scene->Duplicate(prefab);
				Mesh* mesh = static_cast<MeshFilter*>(go->meshFilter)->mesh;
				Scene::scene->renderObjectsLayer[(int)RenderLayer::Opaque][mesh].gameObjects.push_back(go);
				Scene::scene->renderObjectsLayer[(int)RenderLayer::Opaque][mesh].isDirty = true;
				DeletePrefab();
			}
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

	void SelectModel(Mesh* mesh, int matIndex, float size)
	{
		if (prefab) {
			auto meshFilter = prefab->GetComponent<MeshFilter>();
			DeletePrefab();

			if (meshFilter && meshFilter->mesh == mesh)
				return;
		}
		
		prefab = Scene::scene->CreateEmpty();
		prefab->GetComponent<Transform>()->Scale({ size, size, size });
		prefab->AddComponent<MeshFilter>()->mesh = mesh;
		auto renderer = prefab->AddComponent<Renderer>();
		for (auto& sm : mesh->DrawArgs)
			renderer->materials.push_back(matIndex);
		prefab->AddComponent<RotatingBehavior>()->speedRotating = Random::Range(-10.0f, 10.0f) * 2;
		Scene::scene->renderObjectsLayer[(int)RenderLayer::Opaque][mesh].gameObjects.push_back(prefab);
		Scene::scene->renderObjectsLayer[(int)RenderLayer::Opaque][mesh].isDirty = true;
	}

	void DeletePrefab()
	{
		Scene::scene->PushDelete(prefab);
		auto m = prefab->GetComponent<MeshFilter>()->mesh;
		std::vector<GameObject*>& v = Scene::scene->renderObjectsLayer[(int)RenderLayer::Opaque][m].gameObjects;
		v.erase(std::find(v.begin(), v.end(), prefab));
		Scene::scene->renderObjectsLayer[(int)RenderLayer::Opaque][m].isDirty = true;
		prefab = nullptr;
	}
};