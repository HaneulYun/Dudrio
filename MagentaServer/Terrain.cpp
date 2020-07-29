#include "pch.h"
#include "Terrain.h"

Terrain::~Terrain(void)
{
	if (bytes)
		delete[] bytes;
	bytes = NULL;

	if (extraData)
		delete[] extraData;
	extraData = NULL;
}

void Terrain::Load()
{
	float* pHeightMapPixels = new float[heightmapWidth * heightmapHeight];
	HANDLE hFile = ::CreateFile(AlphamapTextureName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
	DWORD dwBytesRead;
	::ReadFile(hFile, pHeightMapPixels, (heightmapWidth * heightmapHeight), &dwBytesRead, NULL);
	::CloseHandle(hFile);

	bytes = new float[heightmapWidth * heightmapHeight];
	for (int y = 0; y < heightmapHeight; y++)
		for (int x = 0; x < heightmapWidth; x++)
			bytes[x + ((heightmapHeight - 1 - y) * heightmapWidth)] = pHeightMapPixels[x + (y * heightmapWidth)];

	if (pHeightMapPixels)
		delete[] pHeightMapPixels;
}

void Terrain::makeExtraData()
{
	//if (extraData != nullptr)
	//	delete[] extraData;
	extraData = new ExtraNodeData[heightmapWidth * heightmapHeight];
}

Vector3D Terrain::GetHeightMapNormal(int x, int z)
{
	if ((x < 0.0f) || (z < 0.0f) || (x >= heightmapWidth) || (z >= heightmapHeight))
		return(Vector3D(0.0f, 1.0f, 0.0f));

	int nHeightMapIndex = x + (z * heightmapWidth);
	int xHeightMapAdd = (x < (heightmapWidth - 1)) ? 1 : -1;
	int zHeightMapAdd = (z < (heightmapHeight - 1)) ? heightmapWidth : -heightmapWidth;

	float y1 = (float)bytes[nHeightMapIndex] / 255.0f * size.y;
	float y2 = (float)bytes[nHeightMapIndex + xHeightMapAdd] / 255.0f * size.y;
	float y3 = (float)bytes[nHeightMapIndex + zHeightMapAdd] / 255.0f * size.y;

	Vector3D xmf3Edge1 = Vector3D(0.0f, y3 - y1, 1.0f);
	Vector3D xmf3Edge2 = Vector3D(1.0f, y2 - y1, 0.0f);
	Vector3D xmf3Normal = crossproduct(xmf3Edge1, xmf3Edge2).Normalize();

	return xmf3Normal;
}

float Terrain::GetHeight(float fx, float fz)
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

