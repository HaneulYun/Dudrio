#include "TerrainGenerator.h"

uint8_t TerrainGenerator::ToUint8(double x)
{
	return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<uint8_t>(x * 255.0 + 0.5);
}

float TerrainGenerator::Fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float TerrainGenerator::Lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float TerrainGenerator::Grad(uint8_t hash, float x, float y, float z)
{
	const uint8_t h = hash & 15;
	const float u = h < 8 ? x : y;
	const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float TerrainGenerator::noise2D(float x, float y)
{
	return noise3D(x, y, 0);
}

float TerrainGenerator::noise3D(float x, float y, float z)
{
	int X = static_cast<int>(floor(x)) & 255;
	int Y = static_cast<int>(floor(y)) & 255;
	int Z = static_cast<int>(floor(z)) & 255;

	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	float u = Fade(x);
	float v = Fade(y);
	float w = Fade(z);

	int A = randomTable[X] + Y;
	int AA = randomTable[A];
	int AB = randomTable[A + 1];
	int B = randomTable[X + 1] + Y;
	int BA = randomTable[B];
	int BB = randomTable[B + 1];

	return Lerp(w, Lerp(v, Lerp(u, Grad(randomTable[AA], x, y, z),
		Grad(randomTable[BA], x - 1, y, z)),
		Lerp(u, Grad(randomTable[AB], x, y - 1, z),
			Grad(randomTable[BB], x - 1, y - 1, z))),
		Lerp(v, Lerp(u, Grad(randomTable[AA + 1], x, y, z - 1),
			Grad(randomTable[BA + 1], x - 1, y, z - 1)),
			Lerp(u, Grad(randomTable[AB + 1], x, y - 1, z - 1),
				Grad(randomTable[BB + 1], x - 1, y - 1, z - 1))));
}

float TerrainGenerator::accumulatedOctaveNoise2D(float x, float y, int octaves)
{
	float result = 0;
	float amp = 1;

	for (int i = 0; i < octaves; ++i)
	{
		result += noise2D(x, y) * amp;
		// 주기는 2배
		x *= 2;
		y *= 2;
		// 진폭은 0.5배
		amp /= 2;
	}

	return result; // unnormalized
}

float TerrainGenerator::accumulatedOctaveNoise2D_0_1(float x, float y, int octaves)
{
	return clamp<float>(accumulatedOctaveNoise2D(x, y, octaves) * 0.5 + 0.5, 0, 1);
}



void TerrainGenerator::initTable(int seed)
{
	for (int i = 0; i < 256; ++i)
	{
		randomTable[i] = static_cast<uint8_t>(i);
	}
	shuffle(begin(randomTable), begin(randomTable) + 256, default_random_engine(seed));

	for (size_t i = 0; i < 256; ++i)
	{
		randomTable[256 + i] = randomTable[i];
	}
}

float curve(float v, float a, float b)
{
	return pow(v, a) / (pow(v, a) + pow((b - b * v), a));
}

void TerrainGenerator::createFallOffData(char shape[])
{
	data_fallOff.resize(width * height);

	if (!strcmp(shape, "circle"))
	{
		float maxValue = width * 0.6f;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				float X = (x - width / 2) / maxValue;
				float Y = (y - height / 2) / maxValue;

				float value = sqrt(X * X + Y * Y);

				data_fallOff[y * width + x] = curve(value, 3, 2);
			}
		}
	}
	else if (!strcmp(shape, "square"))
	{
		float maxValue = width * 0.6f;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				float X = (x - width / 2) / maxValue;
				float Y = (y - height / 2) / maxValue;

				float value = max(abs(X), abs(Y));

				data_fallOff[y * width + x] = curve(value, 3, 2);
			}
		}
	}

}

string TerrainGenerator::createHeightMap(float frequency, int octaves, int seed, char shape[])
{
	initTable(seed);
	createFallOffData(shape);

	float fx = width / frequency;
	float fy = height / frequency;


	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			data[y * width + x] = (accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves) - data_fallOff[y * width + x]) / 4;
		}
	}

	stringstream ss;
	ss << 'f' << frequency << 'o' << octaves << '_' << seed << ".raw";

	vector<uint8_t> rawData;
	for (auto d : data) rawData.push_back(ToUint8(d));

	ofstream os(ss.str(), ios::out | ios::binary);
	os.write((char*)&rawData.front(), rawData.size());
	os.close();

	return ss.str();
}