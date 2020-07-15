#pragma once
#include "main.h"

class TerrainGenerator
{
private:
	int width;
	int height;

	UINT8 randomTable[512];
	vector<float> data;
	vector<float> data_fallOff;
public:
	TerrainGenerator(int width, int height) : width(width), height(height) { data.resize(width * height); };

	void initTable(int seed);

	uint8_t ToUint8(double x);
	float Fade(float t);
	float Lerp(float t, float a, float b);
	float Grad(uint8_t hash, float x, float y, float z);

	float accumulatedOctaveNoise2D_0_1(float x, float y, int octaves);
	float accumulatedOctaveNoise2D(float x, float y, int octaves);
	float noise2D(float x, float y);
	float noise3D(float x, float y, float z);

	void createFallOffData(char shape[]);
	string createHeightMap(float frequency, int octaves, int seed, char shape[]);
};

class Terrain
{
public:
	std::wstring AlphamapTextureName{};
	int heightmapHeight{ 0 };
	int heightmapWidth{ 0 };

	float	x_size, y_size, z_size;
	float	terrain_size;
	float	frequency;
	int		octaves;
	int		seed;

private:
	BYTE* bytes;

public:
	Terrain() = default;
	~Terrain(void)
	{
		if (bytes)
			delete[] bytes;
		bytes = NULL;
	}

	void Load()
	{
		BYTE* pHeightMapPixels = new BYTE[heightmapWidth * heightmapHeight];
		HANDLE hFile = ::CreateFile(AlphamapTextureName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
		DWORD dwBytesRead;
		::ReadFile(hFile, pHeightMapPixels, (heightmapWidth * heightmapHeight), &dwBytesRead, NULL);
		::CloseHandle(hFile);

		bytes = new BYTE[heightmapWidth * heightmapHeight];
		for (int y = 0; y < heightmapHeight; y++)
			for (int x = 0; x < heightmapWidth; x++)
				bytes[x + ((heightmapHeight - 1 - y) * heightmapWidth)] = pHeightMapPixels[x + (y * heightmapWidth)];

		if (pHeightMapPixels)
			delete[] pHeightMapPixels;
	}

	float GetHeight(float fx, float fz)
	{
		if ((fx < 0.0f) || (fz < 0.0f) || (fx >= heightmapWidth) || (fz >= heightmapHeight))
			return(0.0f);

		int x = (int)fx;
		int z = (int)fz;
		float fxPercent = fx - x;
		float fzPercent = fz - z;
		float fBottomLeft = (float)bytes[x + (z * heightmapWidth)];
		float fBottomRight = (float)bytes[(x + 1) + (z * heightmapWidth)];
		float fTopLeft = (float)bytes[x + ((z + 1) * heightmapWidth)];
		float fTopRight = (float)bytes[(x + 1) + ((z + 1) * heightmapWidth)];

		bool bRightToLeft = ((z % 2) != 0);
		if (bRightToLeft)
		{
			if (fzPercent >= fxPercent)
				fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
			else
				fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
		}
		else
		{
			if (fzPercent < (1.0f - fxPercent))
				fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
			else
				fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
		}

		float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
		float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
		float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;

		return(fHeight);
	}
};