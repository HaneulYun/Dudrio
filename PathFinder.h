#pragma once

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

public:
	GameObject* prefab;
	static PathFinder* Instance()
	{
		static PathFinder instance;

		return &instance;
	}

	void SetTerrainData(TerrainData* data)
	{
		terrainData = data;
	}

	Node CreateNode(int x, int z, Node& parentNode, Node& dest)
	{
		Node node;
		node.posX = x;
		node.posZ = z;
		node.parent = &parentNode;
		node.g = parentNode.g + 1;
		node.h = sqrt((dest.posX - x) * (dest.posX - x) + (dest.posZ - z) * (dest.posZ - z));

		return node;
	}

	void FindPath(Vector2 targetPos, Vector2 startPos, deque<Vector2>& path)
	{
		const int height = terrainData->heightmapHeight;
		const int width = terrainData->heightmapWidth;

		const int halfX = 0;
		const int halfZ = 0;

		vector<Node> openList;
		vector<Node> closedList;
		closedList.reserve(400);

		Node start;
		start.posX = (int)startPos.x;
		start.posZ = (int)startPos.y;

		Node dest;
		dest.posX = (int)targetPos.x;
		dest.posZ = (int)targetPos.y;

		openList.push_back(start);
	
		while (!openList.empty())
		{
			// 비용이 가장 적은 노드
			sort(openList.begin(), openList.end());
			Node currentNode = openList.front();
			openList.erase(openList.begin());
			closedList.push_back(currentNode);

			// 목적지를 찾으면 path에 경로를 저장하고 리턴
			if (currentNode == dest)
			{
				while (!(currentNode == start))
				{
					path.push_front(Vector2 { (float)currentNode.posX, (float)currentNode.posZ });
					currentNode = *currentNode.parent;

					auto g = Scene::scene->Duplicate(prefab);
					g->transform->position = Vector3((float)currentNode.posX, terrainData->GetHeight(currentNode.posX, currentNode.posZ), (float)currentNode.posZ);
				}
				return;
			}

			// 새로 갈 수 있는 경로를 추가
			vector<Node> childs;
			Node& parent = closedList.back();
			if (currentNode.posX > -halfX)
			{
				childs.emplace_back(CreateNode(currentNode.posX - 1, currentNode.posZ, parent, dest));
				if (currentNode.posZ > -halfZ)
				{
					childs.emplace_back(CreateNode(currentNode.posX, currentNode.posZ - 1, parent, dest));
					childs.emplace_back(CreateNode(currentNode.posX - 1, currentNode.posZ - 1, parent, dest));
				}
				if (currentNode.posZ < -halfZ + height - 1)
				{
					childs.emplace_back(CreateNode(currentNode.posX, currentNode.posZ + 1, parent, dest));
					childs.emplace_back(CreateNode(currentNode.posX - 1, currentNode.posZ + 1, parent, dest));

				}
			}
			if (currentNode.posX < -halfX + width - 1)
			{
				childs.emplace_back(CreateNode(currentNode.posX + 1, currentNode.posZ, parent, dest));
				if (currentNode.posZ > -halfZ)
				{
					childs.emplace_back(CreateNode(currentNode.posX + 1, currentNode.posZ - 1, parent, dest));
				}
				if (currentNode.posZ < -halfZ + height - 1)
				{
					childs.emplace_back(CreateNode(currentNode.posX + 1, currentNode.posZ + 1, parent, dest));

				}
			}

			for (auto node : childs)
			{
				// 장애물이면
				//if (terrainData->bytes[(node.posX + halfX) + (node.posZ + halfZ) * width] != 0) continue;
				// closedList에 존재하면
				if (find(closedList.begin(), closedList.end(), node) != closedList.end()) continue;
				// openList에 없으면
				auto searchNode = find(openList.begin(), openList.end(), node);
				if (searchNode == openList.end())
					openList.push_back(node);
				// openList에 있으면
				else
				{
					// 새로운 경로의 비용이 더 적으면
					if (searchNode->g > node.g)
					{
						searchNode->g = node.g;
						searchNode->parent = &parent;
					}
				}
			}
		}

		Debug::Log("길 못찾음");
	}

	void MoveToDestination(Vector2& targetPos, Transform* object, float speed)
	{
		Vector3 currentDir{ object->forward.x, 0, object->forward.z }; currentDir.Normalize();
		Vector3 dir{ targetPos.x - object->position.x, 0, targetPos.y - object->position.z };
		
		float angle = NS_Vector3::Angle(currentDir.xmf3, dir.xmf3);

		//if (angle < 0.01f)
		//{
		//	Vector3 newPos = object->position + currentDir.Normalize() * speed * Time::deltaTime;
		//	object->position = { newPos.x, terrainData->GetHeight(newPos.x, newPos.z), newPos.z };
		//}
		//else
		{
			//Vector3 newPos = object->position + currentDir.Normalized() * speed * Time::deltaTime;
			//object->position = { newPos.x, terrainData->GetHeight(newPos.x, newPos.z), newPos.z };

			float rotSpeed = 30.0f * Time::deltaTime;
			if (abs(angle) > rotSpeed)
				angle = angle < 0 ? -rotSpeed : rotSpeed;
			object->Rotate(Vector3{ 0,1,0 }, angle);
			
			Vector3 newPos = object->position + object->forward.Normalized() * speed * Time::deltaTime;
			object->position = { newPos.x, terrainData->GetHeight(newPos.x, newPos.z), newPos.z };
		}
	}
};