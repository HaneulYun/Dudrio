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

	// 시작 위치부터 장애물이 안나오는 장소까지의 경로를 찾는다.
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

		// 장애물이 아니면 해당 경로를 저장하고 마지막 위치 반환
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

		// 비용이 가장 적은 노드
		sort(openList.begin(), openList.end());
		Node currentNode = openList.front();
		openList.erase(openList.begin());
		closedList.push_back(currentNode);

		// 목적지를 찾으면 path에 경로를 저장하고 리턴
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
		if (currentNode == start && terrainData->extraData[currentNode.posX + currentNode.posZ * width].collision)
		{
			for (int i = 0; i < childs.size(); ++i)
			{
				if (!collisionCheckOn || !terrainData->extraData[childs[i].posX + childs[i].posZ * width].collision)
					break;

				// 주변 8칸이 모두 막혔으면
				if (i == childs.size() - 1)
				{
					// 장애물이 안나오는 위치까지 이동하고
					Vector2D newStartPos = Escape(startPos, path);

					// 해당 위치부터 다시 목적지까지의 경로를 찾는다.
					return FindPath(targetPos, newStartPos, path);
				}
			}
		}


		for (auto node : childs)
		{
			// 장애물이면
			if (collisionCheckOn && terrainData->extraData[node.posX + node.posZ * width].collision) continue;
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

	//Debug::Log("길 못찾음");
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