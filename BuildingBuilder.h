#pragma once
#include "..\CyanEngine\framework.h"

class BuildingBuilder : public MonoBehavior<BuildingBuilder>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Count, None = -1 };
private /*이 영역에 private 변수를 선언하세요.*/:
	std::vector<BuildingBuilderData> building[BuildingType::Count];
	GameObject* prefab{ nullptr };

public  /*이 영역에 public 변수를 선언하세요.*/:
	Terrain* terrain{ nullptr };
	float distance;

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingBuilder>;
	BuildingBuilder() = default;
	BuildingBuilder(BuildingBuilder&) = default;

public:
	~BuildingBuilder() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		if (prefab)
		{
			prefab->transform->position = getPosOnTerrain();
		}
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
	void serializeBuildings()
	{
		building[Landmark].push_back(makeBuilderDataAsPrefab(L"Well", ASSET PREFAB("Well")));
	}

	BuildingBuilderData makeBuilderDataAsPrefab(wstring name, GameObject* prefab)
	{
		BuildingBuilderData data{ name };
		data.prefab = prefab;
		return data;
	}

	BuildingBuilderData makeBuilderDataAsAsset(wstring name, Mesh* mesh, Material* material)
	{
		BuildingBuilderData data{ name };
		data.mesh = mesh;
		data.material = material;
		return data;
	}

	void build(int type, int index)
	{
		if (index < building[type].size())
		{
			auto data = building[type][index];
			if (data.prefab)
				prefab = Scene::scene->Duplicate(data.prefab);
			else
			{
				prefab = Scene::scene->Duplicate(ASSET PREFAB("MRC"));
				prefab->GetComponent<MeshFilter>()->mesh = data.mesh;
				prefab->GetComponent<Renderer>()->materials.push_back(data.material);
			}
		}
	}

	wstring getBuildingName(int type, int index)
	{
		if (index < building[type].size())
			return building[type][index].buildingName;
		return L"X";
	}

	Vector3 getPosOnTerrain()
	{
		Vector3 screenPos{ Input::mousePosition.x, Input::mousePosition.y, 1.0f };

		Vector3 rayOrigin{ 0.0f, 0.0f, 0.0f };
		Vector3 rayDir = Camera::main->ScreenToWorldPoint(screenPos);

		Matrix4x4 invView = Camera::main->view.Inverse();
		Matrix4x4 invWorld = terrain->gameObject->transform->localToWorldMatrix.Inverse();
		Matrix4x4 toLocal = invView * invWorld;

		rayOrigin = rayOrigin.TransformCoord(toLocal);
		rayDir = rayDir.TransformNormal(invWorld).Normalize();

		if (IntersectPlane(rayOrigin, rayDir, Vector3{ 0,0,0 }, Vector3{ 1,0,0 }, Vector3{ 0,0,1 }))
		{
			Vector3 point;

			std::vector<XMFLOAT3> vertices;
			point = rayOrigin + rayDir * distance;

			auto terrainMesh = terrain->terrainData.heightmapTexture;
			for (float i = 1; i < distance + 1; i += 0.5)
			{
				Vector3 p = rayOrigin + rayDir * i;
				if ((int)point.x == (int)p.x && (int)point.z == (int)p.z)
					continue;

				point = p;
				float x, y, z;
				if (std::ceil(point.x) > terrain->terrainData.GetHeightMapWidth() - 1 || std::ceil(point.z) > terrain->terrainData.GetHeightMapLength() - 1)
					continue;

				x = std::floor(point.x);
				z = std::floor(point.z);
				vertices.push_back({ x, terrainMesh->OnGetHeight(x, z, &terrain->terrainData), z });
				vertices.push_back({ x + 1, terrainMesh->OnGetHeight(x + 1, z + 1, &terrain->terrainData), z + 1 });
				vertices.push_back({ x + 1, terrainMesh->OnGetHeight(x + 1, z, &terrain->terrainData), z });

				vertices.push_back({ x, terrainMesh->OnGetHeight(x, z, &terrain->terrainData), z });
				vertices.push_back({ x, terrainMesh->OnGetHeight(x, z + 1, &terrain->terrainData), z + 1 });
				vertices.push_back({ x + 1, terrainMesh->OnGetHeight(x + 1, z + 1, &terrain->terrainData), z + 1 });

			}
			IntersectVertices(rayOrigin.xmf3, rayDir.xmf3, vertices);

			point = rayOrigin + rayDir * distance;
			point = point.TransformCoord(terrain->gameObject->transform->localToWorldMatrix);

			return point;
		}
		return { 0 };
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
};