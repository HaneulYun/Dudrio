#include "PathFinder.h"

PathFinder* PathFinder::Instance()
{
	static PathFinder instance;

	return &instance;
}

void PathFinder::SetTerrainData(Terrain* data)
{
	terrainData = data;
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

Vector2D PathFinder::Escape(Vector2D startPos, deque<Vector2D>& path)
{
	const int height = terrainData->heightmapHeight;
	const int width = terrainData->heightmapWidth;

	// ���� ��ġ���� ��ֹ��� �ȳ����� ��ұ����� ��θ� ã�´�.
	std::vector<Vector2D> escapePath[4];

	escapePath[0].push_back(startPos);
	escapePath[1].push_back(startPos);
	escapePath[2].push_back(startPos);
	escapePath[3].push_back(startPos);

	while (true)
	{
		if (escapePath[0].back().x > 0)
			escapePath[0].push_back(Vector2D(escapePath[0].back().x - 1, escapePath[0].back().z));
		if (escapePath[1].back().x < width - 1)
			escapePath[1].push_back(Vector2D(escapePath[1].back().x + 1, escapePath[1].back().z));
		if (escapePath[2].back().z > 0)
			escapePath[2].push_back(Vector2D(escapePath[2].back().x, escapePath[2].back().z - 1));
		if (escapePath[3].back().z > height - 1)
			escapePath[3].push_back(Vector2D(escapePath[3].back().x, escapePath[3].back().z + 1));

		// ��ֹ��� �ƴϸ� �ش� ��θ� �����ϰ� ������ ��ġ ��ȯ
		for (int i = 0; i < 4; ++i)
		{
			if (terrainData->extraData[(int)escapePath[i].back().x + (int)escapePath[i].back().z * width].collision == false)
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

bool PathFinder::FindPath(Vector2D targetPos, Vector2D startPos, deque<Vector2D>& path, bool collisionCheckOn, float targetPosOffset)
{
	const int height = terrainData->heightmapHeight;
	const int width = terrainData->heightmapWidth;

	std::vector<Node> openList;
	std::vector<Node> closedList;
	closedList.reserve(400);

	Node start;
	start.posX = (int)startPos.x;
	start.posZ = (int)startPos.z;

	Node dest;
	dest.posX = (int)targetPos.x;
	dest.posZ = (int)targetPos.z;

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
			std::vector<Vector2D> pathTemp;
			while (!(currentNode == start))
			{
				pathTemp.push_back(Vector2D{ (float)currentNode.posX, (float)currentNode.posZ });
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
		if (currentNode == start && terrainData->extraData[currentNode.posX + currentNode.posZ * width].collision)
		{
			for (int i = 0; i < childs.size(); ++i)
			{
				if (!collisionCheckOn || !terrainData->extraData[childs[i].posX + childs[i].posZ * width].collision)
					break;

				// �ֺ� 8ĭ�� ��� ��������
				if (i == childs.size() - 1)
				{
					// ��ֹ��� �ȳ����� ��ġ���� �̵��ϰ�
					Vector2D newStartPos = Escape(startPos, path);

					// �ش� ��ġ���� �ٽ� ������������ ��θ� ã�´�.
					return FindPath(targetPos, newStartPos, path);
				}
			}
		}


		for (auto node : childs)
		{
			// ��ֹ��̸�
			if (collisionCheckOn && terrainData->extraData[node.posX + node.posZ * width].collision) continue;
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

	//Debug::Log("�� ��ã��");
}

void PathFinder::MoveToDestination(Vector2D& targetPos, Sim* sim, float speed)
{
	float prevX = sim->pos.x; float prevZ = sim->pos.z;
	Vector3D currentDir{ sim->forward.x, 0, sim->forward.z }; currentDir.Normalize();
	Vector3D dir{ targetPos.x - sim->pos.x, 0, targetPos.z - sim->pos.z };

	float rotSpeed = 0.f;

	float angle = getDegreeAngle(currentDir, dir);
	if (angle < 3.f * speed)
	{
		currentDir = dir;
		Vector3D newPos = Vector3D(sim->pos.x, 0, sim->pos.z) + currentDir.Normalize() * speed * 0.333;
		sim->pos = { newPos.x, newPos.z };

		if (sim->is_sector_change(prevX, prevZ)) {
			sim->erase_sim_in_sector(prevX, prevZ);
			sim->insert_client_in_sector();
		}

		iocp.send_move_sim_packet(contents.host_id, sim->id, rotSpeed);

		vector<int> near_clients = sim->get_near_clients();
		for (auto& cl : sim->prevNearClients) {
			lock_guard<mutex>lock_guard(g_clients[cl]->m_cl);
			auto iter = find(near_clients.begin(), near_clients.end(), cl);
			if (iter == near_clients.end()) {
				iocp.send_leave_sim_packet(cl, sim->id);
			}
		}

		for (auto& cl : near_clients) {
			lock_guard<mutex>lock_guard(g_clients[cl]->m_cl);
			if (g_clients[cl]->sim_list.count(sim->id) != 0) {
				iocp.send_move_sim_packet(cl, sim->id, rotSpeed);
			}
			else {
				iocp.send_enter_sim_packet(cl, sim->id);
			}
		}

		sim->prevNearClients = near_clients;
		return;
	}

	Vector3D cross = crossproduct(currentDir, dir);
	Vector3D up = { 0,1,0 };
	bool isRight = dotproduct(cross, up) > 0 ? true : false;

	rotSpeed = 200.f * 0.333 * speed;
	if (!isRight) rotSpeed *= -1;

	sim->rotAngle += rotSpeed;
	sim->forward.Rotate(rotSpeed);

	Vector2D newPos = sim->pos + sim->forward.Normalize() * speed * 0.333;// Time::deltaTime;
	sim->pos = { newPos.x, newPos.z };

	if (sim->is_sector_change(prevX, prevZ)) {
		sim->erase_sim_in_sector(prevX, prevZ);
		sim->insert_client_in_sector();
	}

	iocp.send_move_sim_packet(contents.host_id, sim->id, rotSpeed);

	vector<int> near_clients = sim->get_near_clients();
	for (auto& cl : sim->prevNearClients){
		lock_guard<mutex>lock_guard(g_clients[cl]->m_cl);
		auto iter = find(near_clients.begin(), near_clients.end(), cl);
		if (iter == near_clients.end()) {
			iocp.send_leave_sim_packet(cl, sim->id);
		}
	}

	for (auto& cl : near_clients) {
		lock_guard<mutex>lock_guard(g_clients[cl]->m_cl);
		if (g_clients[cl]->sim_list.count(sim->id) != 0) {
			iocp.send_move_sim_packet(cl, sim->id, rotSpeed);
		}
		else {
			iocp.send_enter_sim_packet(cl, sim->id);
		}
	}

	sim->prevNearClients = near_clients;
	cout << sim->id << " has moved to " << sim->pos.x << ", " << sim->pos.z << endl;
}