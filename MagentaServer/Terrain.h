#pragma once
#include "main.h"

class Terrain
{
public:
	wstring AlphamapTextureName{};
	int heightmapHeight{ 0 };
	int heightmapWidth{ 0 };
	Vector3D size;

	float	terrain_size;
	float	frequency;
	int		octaves;
	int		seed;

private:
	float* bytes;

public:
	Terrain() = default;
	~Terrain(void);

	void Load();

	float GetHeight(float x, float z);
	Vector3D GetHeightMapNormal(int x, int z);

	float* GetHeightMapPixels() { return(bytes); }
	int GetHeightMapWidth() { return(heightmapWidth); }
	int GetHeightMapLength() { return(heightmapHeight); }
};

struct ExtraNodeData
{
	bool collision = 0;
};

class TerrainNodeData
{
public:
	Terrain* data;
	ExtraNodeData* extraData;

	TerrainNodeData(Terrain* terrainData) : data(terrainData)
	{
		extraData = new ExtraNodeData[terrainData->heightmapWidth * terrainData->heightmapHeight];
	}
};