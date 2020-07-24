#pragma once
#include "TerrainNodeData.h"

struct Node
{
	int posX;
	int posZ;

	int g = 0;
	float h = 0;

	Node* parent = NULL;

	constexpr bool operator <(const Node& left) const
	{
		return (g + h < left.g + left.h);
	}

	constexpr bool operator ==(const Node& left) const
	{
		if (posX == left.posX && posZ == left.posZ)
			return true;
		return false;
	}
};

class PathFinder
{
private:
	TerrainData* terrainData;
	TerrainNodeData* terrainNodeData;

public:
	//GameObject* prefab;
	static PathFinder* Instance();

public:

	void SetTerrainData(TerrainData* data, TerrainNodeData* nodeData);

	Node CreateNode(int x, int z, Node& parentNode, Node& dest);

	float distance(Node a, Node b);

	void FindPath(Vector2 targetPos, Vector2 startPos, std::deque<Vector2>& path, float targetPosOffset = 0);

	void MoveToDestination(Vector2& targetPos, Transform* object, float speed);
};