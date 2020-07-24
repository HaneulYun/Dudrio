#pragma once

struct ExtraNodeData
{
	bool canMove;
};

class TerrainNodeData
{
public:
	TerrainData* data;
	ExtraNodeData* extraData;

	TerrainNodeData(TerrainData* terrainData) : data(terrainData)
	{
		extraData = new ExtraNodeData[terrainData->heightmapWidth + terrainData->heightmapWidth]{ false };
	}
};