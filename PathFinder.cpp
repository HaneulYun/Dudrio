#include "pch.h"
#include "PathFinder.h"

PathFinder* PathFinder::Instance()
{
	static PathFinder instance;

	return &instance;
}

void PathFinder::SetTerrainData(TerrainData* data, TerrainNodeData* nodeData)
{
	terrainData = data;
	terrainNodeData = nodeData;
}

Node PathFinder::CreateNode(int x, int z, Node& parentNode, Node& dest)
{
	Node node;
	node.posX = x;
	node.posZ = z;
	node.parent = &parentNode;
	node.g = parentNode.g + 1;
	node.h = sqrt((dest.posX - x) * (dest.posX - x) + (dest.posZ - z) * (dest.posZ - z));

	return node;
}

float PathFinder::distance(Node a, Node b)
{
	return sqrt((a.posX - b.posX) * (a.posX - b.posX) + (a.posZ - b.posZ) * (a.posZ - b.posZ));
}

Vector2 PathFinder::Escape(Vector2 startPos, std::deque<Vector2>& path)
{
	const int height = terrainData->heightmapHeight;
	const int width = terrainData->heightmapWidth;

	// 시작 위치부터 장애물이 안나오는 장소까지의 경로를 찾는다.
	std::vector<Vector2> escapePath[4];

	escapePath[0].push_back(startPos);
	escapePath[1].push_back(startPos);
	escapePath[2].push_back(startPos);
	escapePath[3].push_back(startPos);

	while (true)
	{
		if (escapePath[0].back().x > 0)
			escapePath[0].push_back(Vector2(escapePath[0].back().x - 1, escapePath[0].back().y));
		if (escapePath[1].back().x < width - 1)
			escapePath[1].push_back(Vector2(escapePath[1].back().x + 1, escapePath[1].back().y));
		if (escapePath[2].back().y > 0)
			escapePath[2].push_back(Vector2(escapePath[2].back().x, escapePath[2].back().y - 1));
		if (escapePath[3].back().y > height - 1)
			escapePath[3].push_back(Vector2(escapePath[3].back().x, escapePath[3].back().y + 1));

		// 장애물이 아니면 해당 경로를 저장하고 마지막 위치 반환
		for (int i = 0; i < 4; ++i)
		{
			if (terrainNodeData->extraData[(int)escapePath[i].back().x + (int)escapePath[i].back().y * width].collision == false)
			{
				while (!escapePath[i].empty())
				{
					path.push_front(escapePath[i].back());
					escapePath[i].pop_back();
				}
				return path.back();
			}
		}
	}

}

bool PathFinder::FindPath(Vector2 targetPos, Vector2 startPos, std::deque<Vector2>& path, bool collisionCheckOn, float targetPosOffset)
{
	const int height = terrainData->heightmapHeight;
	const int width = terrainData->heightmapWidth;

	std::vector<Node> openList;
	std::vector<Node> closedList;
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
		if (closedList.size() > 1000)
			return false;

		// 비용이 가장 적은 노드
		sort(openList.begin(), openList.end());
		Node currentNode = openList.front();
		openList.erase(openList.begin());
		closedList.push_back(currentNode);

		// 목적지를 찾으면 path에 경로를 저장하고 리턴
		if ((distance(currentNode, dest) - targetPosOffset) <= 0.01f)
		{
			std::vector<Vector2> pathTemp;
			while (!(currentNode == start))
			{
				pathTemp.push_back(Vector2{ (float)currentNode.posX, (float)currentNode.posZ });
				currentNode = *currentNode.parent;

				//auto d = Scene::scene->Duplicate(prefab);
				//d->transform->position = Vector3(currentNode.posX, terrainData->GetHeight(currentNode.posX, currentNode.posZ), currentNode.posZ);
			}
			while (!pathTemp.empty())
			{
				path.push_back(pathTemp.back());
				pathTemp.pop_back();
			}
			return true;
		}

		// 새로 갈 수 있는 경로를 추가
		std::vector<Node> childs;
		Node& parent = closedList.back();
		if (currentNode.posX > 0)
		{
			childs.emplace_back(CreateNode(currentNode.posX - 1, currentNode.posZ, parent, dest));
			if (currentNode.posZ > 0)
			{
				childs.emplace_back(CreateNode(currentNode.posX, currentNode.posZ - 1, parent, dest));
				childs.emplace_back(CreateNode(currentNode.posX - 1, currentNode.posZ - 1, parent, dest));
			}
			if (currentNode.posZ < height - 1)
			{
				childs.emplace_back(CreateNode(currentNode.posX, currentNode.posZ + 1, parent, dest));
				childs.emplace_back(CreateNode(currentNode.posX - 1, currentNode.posZ + 1, parent, dest));

			}
		}
		if (currentNode.posX < width - 1)
		{
			childs.emplace_back(CreateNode(currentNode.posX + 1, currentNode.posZ, parent, dest));
			if (currentNode.posZ > 0)
			{
				childs.emplace_back(CreateNode(currentNode.posX + 1, currentNode.posZ - 1, parent, dest));
			}
			if (currentNode.posZ < height - 1)
			{
				childs.emplace_back(CreateNode(currentNode.posX + 1, currentNode.posZ + 1, parent, dest));

			}
		}

		// 시작 위치에서 심이 장애물에 끼였는지 체크
		if (currentNode == start && terrainNodeData->extraData[currentNode.posX + currentNode.posZ * width].collision)
		{
			for (int i = 0; i < childs.size(); ++i)
			{
				if (!collisionCheckOn || !terrainNodeData->extraData[childs[i].posX + childs[i].posZ * width].collision)
					break;

				// 주변 8칸이 모두 막혔으면
				if (i == childs.size() - 1)
				{
					// 장애물이 안나오는 위치까지 이동하고
					Vector2 newStartPos = Escape(startPos, path);

					// 해당 위치부터 다시 목적지까지의 경로를 찾는다.
					return FindPath(targetPos, newStartPos, path);
				}
			}
		}


		for (auto node : childs)
		{
			// 장애물이면
			if (collisionCheckOn && terrainNodeData->extraData[node.posX + node.posZ * width].collision) continue;
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

void PathFinder::MoveToDestination(Vector2& targetPos, Transform* object, float speed)
{
	Vector3 currentDir{ object->forward.x, 0, object->forward.z }; currentDir.Normalize();
	Vector3 dir{ targetPos.x - object->position.x, 0, targetPos.y - object->position.z };

	float angle = XMConvertToDegrees(Vector3::Angle(currentDir, dir));
	if (angle < 3.f * speed)
	{
		currentDir = dir;
		Vector3 newPos = object->position + currentDir.Normalize() * speed * Time::deltaTime;
		object->position = { newPos.x, terrainData->GetHeight(newPos.x, newPos.z), newPos.z };

		return;
	}

	Vector3 cross = Vector3::CrossProduct(currentDir, dir);
	Vector3 up = { 0,1,0 };
	bool isRight = Vector3::DotProduct(cross, up) > 0 ? true : false;


	float rotSpeed = 180.f * Time::deltaTime * speed;
	if (!isRight) rotSpeed *= -1;

	object->Rotate(Vector3{ 0,1,0 }, rotSpeed);

	Vector3 newPos = object->position + Vector3::Normalize(object->forward) * speed * 0.016f;// Time::deltaTime;
	object->position = { newPos.x, terrainData->GetHeight(newPos.x, newPos.z), newPos.z };
}