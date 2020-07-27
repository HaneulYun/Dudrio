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

public:
	TerrainData* terrainData;
	TerrainNodeData* terrainNodeData;

	//GameObject* prefab;
	static PathFinder* Instance();

public:

	void SetTerrainData(TerrainData* data, TerrainNodeData* nodeData);

	Node CreateNode(int x, int z, Node& parentNode, Node& dest);

	float distance(Node a, Node b);

	Vector2 Escape(Vector2 startPos, std::deque<Vector2>& path);
	bool FindPath(Vector2 targetPos, Vector2 startPos, std::deque<Vector2>& path, bool collisionCheckOn = true, float targetPosOffset = 0);

	void MoveToDestination(Vector2& targetPos, Transform* object, float speed);
};