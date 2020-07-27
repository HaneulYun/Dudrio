#pragma once
#include "..\CyanEngine\framework.h"
#include "HostNetwork.h"
#include "TerrainNodeData.h"

class BuildingBuilder : public MonoBehavior<BuildingBuilder>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Prop, Count, None = -1 };
private /*�� ������ private ������ �����ϼ���.*/:
	std::vector<BuildingBuilderData> building[BuildingType::Count];
	GameObject* prefab{ nullptr };

public  /*�� ������ public ������ �����ϼ���.*/:
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

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		buildingBuilder = this;
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);

	void serializeBuildings();

	BuildingBuilderData makeBuilderDataAsPrefab(wstring name, GameObject* prefab);

	BuildingBuilderData makeBuilderDataAsMeshAndMaterial(wstring name, Mesh* mesh, Material* material);
	
	BuildingBuilderData makeBuilderDataAsMeshAndMaterials(wstring name, Mesh* mesh, vector<Material*> materials);

	void updateTerrainNodeData(GameObject* building);

	void build(Vector2 position, float angle, int type, int index);

	void build(Vector3 position);

	void exitBuildMode();

	void enterBuildMode(int type, int index);

	float getBoundingBox(int type, int index);

	int getBuildingCount(int type);

	wstring getBuildingName(int type, int index);

	Vector3 getPosOnTerrain();

	bool IntersectPlane(Vector3 rayOrigin, Vector3 rayDirection, Vector3 v0, Vector3 v1, Vector3 v2);

	void IntersectVertices(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, std::vector<XMFLOAT3>& vertices);
};