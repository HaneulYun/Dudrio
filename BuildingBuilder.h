#pragma once
#include "..\CyanEngine\framework.h"
#include "HostNetwork.h"
#include "TerrainNodeData.h"

class BuildingBuilder : public MonoBehavior<BuildingBuilder>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Count, None = -1 };
private /*이 영역에 private 변수를 선언하세요.*/:
	std::vector<BuildingBuilderData> building[BuildingType::Count];
	GameObject* prefab{ nullptr };

public  /*이 영역에 public 변수를 선언하세요.*/:
	static BuildingBuilder* buildingBuilder;

	int curPrefabType;
	int curPrefabIndex;
	float curPrefabAngle;

	Terrain* terrain{ nullptr };
	TerrainNodeData* terrainNodeData; 
	//GameObject* cube;
	float distance;


	Vector3 lastMousePos;
	bool rotationToggle = false;

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingBuilder>;
	BuildingBuilder() = default;
	BuildingBuilder(BuildingBuilder&) = default;

public:
	~BuildingBuilder() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		buildingBuilder = this;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		if (prefab)
		{
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
			else if (Input::GetMouseButtonUp(0))
			{
				auto p = prefab->transform->position;
				
				if (HostNetwork::network->isConnect) 
					HostNetwork::network->send_construct_packet(curPrefabType, curPrefabIndex, p.x, p.z, curPrefabAngle);
				updateTerrainNodeData(prefab);
				
				prefab = nullptr;
			}
			else
				prefab->transform->position = getPosOnTerrain();
		}
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
	void serializeBuildings()
	{
		building[Landmark].push_back(makeBuilderDataAsPrefab(L"Well", ASSET PREFAB("Well")));

		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"House01",			ASSET MESH("SM_House_Var01"),		ASSET MATERIAL("house01")));
		building[House].push_back(makeBuilderDataAsPrefab(L"House02", ASSET PREFAB("House01")));

		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_01",		ASSET MESH("SM_Tree_Var01"),		{ ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_02",		ASSET MESH("SM_Tree_Var02"),		{ ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_03",		ASSET MESH("SM_Tree_Var03"),		{ ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_04",		ASSET MESH("SM_Tree_Var04"),		{ ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_05",		ASSET MESH("SM_Tree_Var05"),		{ ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));

		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_01",	ASSET MESH("SM_Mushroom_Var01"),	ASSET MATERIAL("material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_02",	ASSET MESH("SM_Mushroom_Var02"),	ASSET MATERIAL("material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_03",	ASSET MESH("SM_Mushroom_Var03"),	ASSET MATERIAL("material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_04",	ASSET MESH("SM_Mushroom_Var04"),	ASSET MATERIAL("material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_05",	ASSET MESH("SM_Mushroom_Var05"),	ASSET MATERIAL("material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_06",	ASSET MESH("SM_Mushroom_Var06"),	ASSET MATERIAL("material_02")));

		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_01",			ASSET MESH("SM_Fence_Var01"),		ASSET MATERIAL("material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_02",			ASSET MESH("SM_Fence_Var02"),		ASSET MATERIAL("material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_03",			ASSET MESH("SM_Fence_Var03"),		ASSET MATERIAL("material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_04",			ASSET MESH("SM_Fence_Var04"),		ASSET MATERIAL("material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Bucket",			ASSET MESH("SM_Bucket"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Barrel",			ASSET MESH("SM_Barrel"),			ASSET MATERIAL("material_01")));
		//building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Spike",			ASSET MESH("SM_Mushroom_Var06"),	ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Cart_01",			ASSET MESH("SM_Cart_Var01"),		ASSET MATERIAL("material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Cart_02",			ASSET MESH("SM_Cart_Var02"),		ASSET MATERIAL("material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Cauldron",			ASSET MESH("SM_Cauldron"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_01",			ASSET MESH("SM_Pot_Var01"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_02",			ASSET MESH("SM_Pot_Var02"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_03",			ASSET MESH("SM_Pot_Var03"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_04",			ASSET MESH("SM_Pot_Var04"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_05",			ASSET MESH("SM_Pot_Var05"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_06",			ASSET MESH("SM_Pot_Var06"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Apple",				ASSET MESH("SM_Apple"),				ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Potato",			ASSET MESH("SM_Potato"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Tomato",			ASSET MESH("SM_Tomato"),			ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Watermellon",		ASSET MESH("SM_Watermellon"),		ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Apple",		ASSET MESH("SM_Sack_Apple"),		ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Flour",		ASSET MESH("SM_Sack_Flour"),		ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Potato",		ASSET MESH("SM_Sack_Potato"),		ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Tomato",		ASSET MESH("SM_Sack_Tomato"),		ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_01",			ASSET MESH("SM_Sack_Var01"),		ASSET MATERIAL("material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_02",			ASSET MESH("SM_Sack_Var02"),		ASSET MATERIAL("material_02")));

		//ofstream out("colliders.txt");
		//for (int i = 0; i < BuildingType::Count; ++i) {
		//	int j = 0;
		//	for (auto b : building[i]) {
		//		if (b.prefab) {
		//			auto box = b.prefab->GetComponent<BoxCollider>();
		//			out << i << " " << j << " " << box->center.x - box->extents.x << " " << box->center.y - box->extents.y << " " << box->center.x + box->extents.x << " " << box->center.y + box->extents.y << endl;
		//		}
		//		else {
		//			auto box = b.mesh->Bounds;
		//			out << i << " " << j << " " << box.Center.x - box.Extents.x << " " << box.Center.x - box.Extents.y << " " << box.Center.x + box.Extents.x << " " << box.Center.x + box.Extents.y << endl;
		//		}
		//		++j;
		//	}
		//}
		//out.close();
	}

	BuildingBuilderData makeBuilderDataAsPrefab(wstring name, GameObject* prefab)
	{
		BuildingBuilderData data{ name };
		data.prefab = prefab;
		return data;
	}

	BuildingBuilderData makeBuilderDataAsMeshAndMaterial(wstring name, Mesh* mesh, Material* material)
	{
		BuildingBuilderData data{ name };
		data.mesh = mesh;
		data.material = material;
		return data;
	}
	
	BuildingBuilderData makeBuilderDataAsMeshAndMaterials(wstring name, Mesh* mesh, vector<Material*> materials)
	{
		BuildingBuilderData data{ name };
		data.mesh = mesh;
		data.materials = materials;
		return data;
	}

	void updateTerrainNodeData(GameObject* building)
	{
		BoundingBox boundingBox = building->GetComponent<BoxCollider>()->boundingBox;

		Vector3 pos = building->transform->position;
		Vector3 right = Vector3::Normalize(building->transform->right) + boundingBox.Extents.x;
		Vector3 forward = Vector3::Normalize(building->transform->forward) + boundingBox.Extents.z;

		for (int x = (pos - right).x; x <= (pos + right).x; ++x)
		{
			for (int z = (pos - forward).z; z <= (pos + forward).z; ++z)
			{

				BoundingOrientedBox obbBox{};
				obbBox.Center = pos.xmf3;
				obbBox.Extents = boundingBox.Extents;
				obbBox.Orientation = building->transform->localToWorldMatrix.QuaternionRotationMatrix().xmf4;

				if (obbBox.Contains(XMLoadFloat3(&XMFLOAT3(x, pos.y, z))))
				{
					terrainNodeData->extraData[x + (z * terrain->terrainData.heightmapHeight)].collision = true;

					// 노드 확인용
					//if (cube)
					//{
					//	auto go = Scene::scene->Duplicate(cube);
					//	go->transform->position = Vector3(x, terrain->terrainData.GetHeight(x, z), z);
					//}
				}
			}
		}
		
	}

	void build(Vector2 position, float angle, int type, int index)
	{
		if (index < building[type].size())
		{
			GameObject* obj;

			auto data = building[type][index];
			if (data.prefab)
				obj = Scene::scene->Duplicate(data.prefab);
			else
			{
				obj = Scene::scene->CreateEmpty();
				obj->AddComponent<BoxCollider>()->boundingBox = data.mesh->Bounds;

				auto child = obj->AddChild();
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				child->AddComponent<MeshFilter>()->mesh = data.mesh;
				if (data.material)
					child->AddComponent<Renderer>()->materials.push_back(data.material);
				else
				{
					auto renderer = child->AddComponent<Renderer>();
					int i = 0;
					for (auto& sm : data.mesh->DrawArgs)
						renderer->materials.push_back(data.materials[i++]);
				}
			}

			Vector3 pos{ position.x, terrain->terrainData.GetHeight(position.x,position.y), position.y };
			obj->transform->position = pos;
			obj->transform->Rotate(Vector3(0, 1, 0), angle);

			updateTerrainNodeData(obj);
		}
	}

	void build(Vector3 position)
	{
		prefab->transform->position = position;
		prefab = nullptr;
	}

	void exitBuildMode()
	{

	}

	void enterBuildMode(int type, int index)
	{
		if (index < building[type].size())
		{
			auto data = building[type][index];
			if (data.prefab)
				prefab = Scene::scene->Duplicate(data.prefab);
			else
			{
				prefab = Scene::scene->CreateEmpty();
				prefab->AddComponent<BoxCollider>()->boundingBox = data.mesh->Bounds;

				auto child = prefab->AddChild();
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				child->AddComponent<MeshFilter>()->mesh = data.mesh;
				if (data.material)
					child->AddComponent<Renderer>()->materials.push_back(data.material);
				else
				{
					auto renderer = child->AddComponent<Renderer>();
					int i = 0;
					for (auto& sm : data.mesh->DrawArgs)
						renderer->materials.push_back(data.materials[i++]);
				}
			}

			curPrefabType = type;
			curPrefabIndex = index;
		}
	}

	float getBoundingBox(int type, int index)
	{
		if (index < building[type].size())
		{
			auto data = building[type][index];

			XMFLOAT3 bound;

			if (data.prefab)
				bound = data.prefab->GetComponent<BoxCollider>()->boundingBox.Extents;
			else
				bound = data.mesh->Bounds.Extents;

			return bound.x > bound.y ? bound.x : bound.y;
		}
		return 0;
	}

	int getBuildingCount(int type)
	{
		return building[type].size();
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