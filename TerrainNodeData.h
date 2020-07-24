#pragma once

struct ExtraNodeData
{
	bool collision = 0;
};

class TerrainNodeData
{
public:
	TerrainData* data;
	ExtraNodeData* extraData;

	TerrainNodeData(TerrainData* terrainData) : data(terrainData)
	{
		extraData = new ExtraNodeData[terrainData->heightmapWidth * terrainData->heightmapHeight];
	}
};