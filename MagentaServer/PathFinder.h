#pragma once
#include "Terrain.h"

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
	Terrain* terrainData;

	//GameObject* prefab;
	static PathFinder* Instance();

public:

	void SetTerrainData(Terrain* data);

	Node CreateNode(int x, int z, Node& parentNode, Node& dest);

	float distance(Node a, Node b);

	Vector2D Escape(Vector2D startPos, std::deque<Vector2D>& path);
	bool FindPath(Vector2D targetPos, Vector2D startPos, std::deque<Vector2D>& path, bool collisionCheckOn = true, float targetPosOffset = 0);

	void MoveToDestination(Vector2D& targetPos, Sim* sim, float speed);
};