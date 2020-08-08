#pragma once
#include "..\CyanEngine\framework.h"
#include "HostNetwork.h"
#include "TerrainNodeData.h"

class BuildingBuilder : public MonoBehavior<BuildingBuilder>
{
public:
	enum BuilderMode { DefaultMode, BuildMode, DeleteMode, GuestMode };
	enum BuildingType { Landmark, House, Theme, Landscape, Lighting, Fence, Prop, Nature, Count, None = -1 };
private /*�� ������ private ������ �����ϼ���.*/:
	std::vector<BuildingBuilderData> building[BuildingType::Count];
	GameObject* prefab{ nullptr };

public  /*�� ������ public ������ �����ϼ���.*/:
	static BuildingBuilder* buildingBuilder;

	GameObject* curLandmark;
	int curPrefabType = -1;
	int curPrefabIndex = -1;

	Terrain* terrain{ nullptr };
	TerrainNodeData* terrainNodeData; 
	GameObject* cube;
	float distance;


	Vector3 lastMousePos;
	bool rotationToggle = false;
	BuilderMode builderMode = DefaultMode;

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingBuilder>;
	BuildingBuilder() = default;
	BuildingBuilder(BuildingBuilder&) = default;

public:
	~BuildingBuilder() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/);

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/);

	void serializeBuildings();

	BuildingBuilderData makeBuilderDataAsPrefab(wstring name, GameObject* prefab);

	BuildingBuilderData makeBuilderDataAsMeshAndMaterial(wstring name, Mesh* mesh, Material* material);
	
	BuildingBuilderData makeBuilderDataAsMeshAndMaterials(wstring name, Mesh* mesh, vector<Material*> materials);

	GameObject* isOnLand();

	void updateTerrainNodeData(GameObject* building, bool collision);

	void initNature();

	void build(Vector2 position, double angle, int type, int index, GameObject* landmark = nullptr);

	void build(Vector3 position);

	void simBuild(Vector2 position, double angle, int type, int index, GameObject* landmark);

	void makePrefab(int type, int index);

	void exitBuildMode();

	void enterBuildMode(int type, int index);

	void exitDeleteMode();

	int enterDeleteMode();

	float getBoundingBox(int type, int index);

	int getBuildingCount(int type);

	wstring getBuildingName(int type, int index);

	int getLandmarkRaduis(GameObject* landmark);

	void pickToDelete();

	Vector3 getPosOnTerrain();

	bool IntersectPlane(Vector3 rayOrigin, Vector3 rayDirection, Vector3 v0, Vector3 v1, Vector3 v2);

	void IntersectVertices(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, std::vector<XMFLOAT3>& vertices);

	void hostLoad(int type, int index, float x, float z, float angle, int range, bool development);
	void guestBuild(int type, int index, float x, float z, float angle, int range);
};