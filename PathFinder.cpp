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

	// ���� ��ġ���� ��ֹ��� �ȳ����� ��ұ����� ��θ� ã�´�.
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

		// ��ֹ��� �ƴϸ� �ش� ��θ� �����ϰ� ������ ��ġ ��ȯ
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

		// ����� ���� ���� ���
		sort(openList.begin(), openList.end());
		Node currentNode = openList.front();
		openList.erase(openList.begin());
		closedList.push_back(currentNode);

		// �������� ã���� path�� ��θ� �����ϰ� ����
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

		// ���� �� �� �ִ� ��θ� �߰�
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

		// ���� ��ġ���� ���� ��ֹ��� �������� üũ
		if (currentNode == start && terrainNodeData->extraData[currentNode.posX + currentNode.posZ * width].collision)
		{
			for (int i = 0; i < childs.size(); ++i)
			{
				if (!collisionCheckOn || !terrainNodeData->extraData[childs[i].posX + childs[i].posZ * width].collision)
					break;

				// �ֺ� 8ĭ�� ��� ��������
				if (i == childs.size() - 1)
				{
					// ��ֹ��� �ȳ����� ��ġ���� �̵��ϰ�
					Vector2 newStartPos = Escape(startPos, path);

					// �ش� ��ġ���� �ٽ� ������������ ��θ� ã�´�.
					return FindPath(targetPos, newStartPos, path);
				}
			}
		}


		for (auto node : childs)
		{
			// ��ֹ��̸�
			if (collisionCheckOn && terrainNodeData->extraData[node.posX + node.posZ * width].collision) continue;
			// closedList�� �����ϸ�
			if (find(closedList.begin(), closedList.end(), node) != closedList.end()) continue;
			// openList�� ������
			auto searchNode = find(openList.begin(), openList.end(), node);
			if (searchNode == openList.end())
				openList.push_back(node);
			// openList�� ������
			else
			{
				// ���ο� ����� ����� �� ������
				if (searchNode->g > node.g)
				{
					searchNode->g = node.g;
					searchNode->parent = &parent;
				}
			}
		}
	}

	Debug::Log("�� ��ã��");
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