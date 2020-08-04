#pragma once
#include "..\CyanEngine\framework.h"
#include "HostNetwork.h"
#include "TerrainNodeData.h"

class BuildingBuilder : public MonoBehavior<BuildingBuilder>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Lighting, Fence, Prop, Count, None = -1 };
private /*이 영역에 private 변수를 선언하세요.*/:
	std::vector<BuildingBuilderData> building[BuildingType::Count];
	GameObject* prefab{ nullptr };

public  /*이 영역에 public 변수를 선언하세요.*/:
	static BuildingBuilder* buildingBuilder;

	GameObject* curLandmark;
	int curPrefabType;
	int curPrefabIndex;

	Terrain* terrain{ nullptr };
	TerrainNodeData* terrainNodeData; 
	GameObject* cube;
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

	void Start(/*초기화 코드를 작성하세요.*/);

	void Update(/*업데이트 코드를 작성하세요.*/);

	void serializeBuildings();

	BuildingBuilderData makeBuilderDataAsPrefab(wstring name, GameObject* prefab);

	BuildingBuilderData makeBuilderDataAsMeshAndMaterial(wstring name, Mesh* mesh, Material* material);
	
	BuildingBuilderData makeBuilderDataAsMeshAndMaterials(wstring name, Mesh* mesh, vector<Material*> materials);

	GameObject* isOnLand();

	void updateTerrainNodeData(GameObject* building, bool collision);

	void build(Vector2 position, double angle, int type, int index, GameObject* landmark = nullptr);

	void build(Vector3 position);

	void makePrefab(int type, int index);

	void exitBuildMode();

	void enterBuildMode(int type, int index);

	float getBoundingBox(int type, int index);

	int getBuildingCount(int type);

	wstring getBuildingName(int type, int index);

	void enterDeleteMode();

	void pickToDelete();

	Vector3 getPosOnTerrain();

	bool IntersectPlane(Vector3 rayOrigin, Vector3 rayDirection, Vector3 v0, Vector3 v1, Vector3 v2);

	void IntersectVertices(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, std::vector<XMFLOAT3>& vertices);

	void hostLoad(int type, int index, float x, float z, float angle, int range);
	void guestBuild(int type, int index, float x, float z, float angle, int range);
};