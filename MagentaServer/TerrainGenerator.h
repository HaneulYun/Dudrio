#pragma once
#include "main.h"

class TerrainGenerator
{
private:
	int width;
	int height;

	UINT8 randomTable[256];
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