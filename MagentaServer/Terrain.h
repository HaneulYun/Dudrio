#pragma once

struct ExtraNodeData
{
	bool collision = 0;
};

class Terrain
{
public:
	wstring AlphamapTextureName{};
	int heightmapHeight{ 0 };
	int heightmapWidth{ 0 };
	Vector3D size;

	ExtraNodeData* extraData;

	float	terrain_size;
	float	frequency;
	int		octaves;
	int		seed;

private:
	float*	bytes;

public:
	Terrain() = default;
	~Terrain(void);

	void Load();
	void makeExtraData();

	float GetHeight(float x, float z);
	Vector3D GetHeightMapNormal(int x, int z);

	float* GetHeightMapPixels() { return(bytes); }
	int GetHeightMapWidth() { return(heightmapWidth); }
	int GetHeightMapLength() { return(heightmapHeight); }

};