#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class Builder : public MonoBehavior<Builder>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	GameObject* prefab{ nullptr };
	deque<BuildingInform> buildCommand;
	deque<BuildingInform> destroyCommand;
public  /*이 영역에 public 변수를 선언하세요.*/:
	static Builder* builder;

private:
	friend class GameObject;
	friend class MonoBehavior<Builder>;
	Builder() = default;
	Builder(Builder&) = default;

public:
	~Builder() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		while (!buildCommand.empty())
		{
			Mesh* mesh = NULL;
			Material* mat = NULL;
			float scaleSize = 0.01f;
			float colliderSize = NULL;

			BuildingInform frontInform = buildCommand.front();
			switch (frontInform.buildingType)
			{
			case BuildingType::House_01:
				mesh = ASSET MESH("SM_House_Var01");
				mat = ASSET MATERIAL("house01");
				colliderSize = 2.75f;
				break;
			case BuildingType::Mushroom_01:
				mesh = ASSET MESH("SM_Mushroom_Var01");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.2f;
				break;
			case BuildingType::Mushroom_02:
				mesh = ASSET MESH("SM_Mushroom_Var02");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.2f;
				break;
			case BuildingType::Mushroom_03:
				mesh = ASSET MESH("SM_Mushroom_Var03");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.2f;
				break;
			case BuildingType::Mushroom_04:
				mesh = ASSET MESH("SM_Mushroom_Var04");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.2f;
				break;
			case BuildingType::Mushroom_05:
				mesh = ASSET MESH("SM_Mushroom_Var05");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.15f;
				break;
			case BuildingType::Mushroom_06:
				mesh = ASSET MESH("SM_Mushroom_Var06");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.15f;
				break;
			case BuildingType::Bucket:
				mesh = ASSET MESH("SM_Bucket");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			case BuildingType::Barrel:
				mesh = ASSET MESH("SM_Barrel");
				mat = ASSET MATERIAL("material_01");
				colliderSize = 0.75f;
				break;
			case BuildingType::Cauldron:
				mesh = ASSET MESH("SM_Cauldron");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.5f;
				break;
			case BuildingType::Pot_01:
				mesh = ASSET MESH("SM_Pot_Var01");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			case BuildingType::Pot_02:
				mesh = ASSET MESH("SM_Pot_Var02");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			case BuildingType::Pot_03:
				mesh = ASSET MESH("SM_Pot_Var03");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			case BuildingType::Pot_04:
				mesh = ASSET MESH("SM_Pot_Var04");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			case BuildingType::Pot_05:
				mesh = ASSET MESH("SM_Pot_Var05");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			case BuildingType::Pot_06:
				mesh = ASSET MESH("SM_Pot_Var06");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			case BuildingType::Apple:
				mesh = ASSET MESH("SM_Apple");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.1f;
				break;
			case BuildingType::Potato:
				mesh = ASSET MESH("SM_Potato");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.1f;
				break;
			case BuildingType::Tomato:
				mesh = ASSET MESH("SM_Tomato");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.15f;
				break;
			case BuildingType::Watermellon:
				mesh = ASSET MESH("SM_Watermellon");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.3f;
				break;
			case BuildingType::Sack_Apple:
				mesh = ASSET MESH("SM_Sack_Apple");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.5f;
				break;
			case BuildingType::Sack_Flour:
				mesh = ASSET MESH("SM_Sack_Flour");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.5f;
				break;
			case BuildingType::Sack_Potato:
				mesh = ASSET MESH("SM_Sack_Potato");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.5f;
				break;
			case BuildingType::Sack_Tomato:
				mesh = ASSET MESH("SM_Sack_Tomato");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.5f;
				break;
			case BuildingType::Sack_01:
				mesh = ASSET MESH("SM_Sack_Var01");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.5f;
				break;
			case BuildingType::Sack_02:
				mesh = ASSET MESH("SM_Sack_Var02");
				mat = ASSET MATERIAL("material_02");
				colliderSize = 0.4f;
				break;
			}

			switch (frontInform.buildingType)
			{
			case BuildingType::Well_01:
			{
				prefab = Scene::scene->CreateEmpty();
				prefab->AddComponent<Building>();
				BoxCollider* collider = prefab->AddComponent<BoxCollider>();
				collider->extents = { 1.5f, 1.8f, 1.5f };
				collider->obb = true;
				{
					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_Well");
					child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("material_03"));
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->layer = (int)RenderLayer::Opaque;
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
				{
					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_Well_Extra02");
					child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("material_03"));
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->layer = (int)RenderLayer::Opaque;
					child->transform->position = { 0.0f,1.5f,0.0f };
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
				{
					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_Well_Extra03");
					child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("material_03"));
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->layer = (int)RenderLayer::Opaque;
					child->transform->position = { 0.0f,1.0f,0.0f };
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
			}
			break;
			case BuildingType::House_02:
			{
				prefab = Scene::scene->CreateEmpty();
				prefab->AddComponent<Building>();
				BoxCollider* collider = prefab->AddComponent<BoxCollider>();
				collider->extents = { 2.5f, 4.5f, 2.5f };
				collider->obb = true;
				{
					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var02");
					child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house02"));
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->layer = (int)RenderLayer::Opaque;
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
				{
					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = ASSET MESH("SM_House_Var02_Extra");
					child->AddComponent<Renderer>()->materials.push_back(ASSET MATERIAL("house02"));
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->AddComponent<RotatingBehavior>()->speedRotating = -10.0f;
					child->layer = (int)RenderLayer::Opaque;
					child->transform->position = { 0.25f,6.0f,2.5f };
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
			}
			break;
			case BuildingType::Fence_01:
				mesh = ASSET MESH("SM_Fence_Var01");
			case BuildingType::Fence_02:
				if (mesh == NULL) mesh = ASSET MESH("SM_Fence_Var02");
				mat = ASSET MATERIAL("material_01");
				{
					prefab = Scene::scene->CreateEmpty();
					prefab->AddComponent<Building>();
					BoxCollider* collider = prefab->AddComponent<BoxCollider>();
					collider->extents = { 1.6f, 0.5f, 0.4f };
					collider->obb = true;

					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = mesh;
					Renderer* renderer = child->AddComponent<Renderer>();
					for (auto& sm : mesh->DrawArgs)
						renderer->materials.push_back(mat);
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->layer = (int)RenderLayer::Opaque;
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
				break;
			case BuildingType::Fence_03:
				mesh = ASSET MESH("SM_Fence_Var03");
			case BuildingType::Fence_04:
				if (mesh == NULL) mesh = ASSET MESH("SM_Fence_Var04");
				mat = ASSET MATERIAL("material_01");
				{
					prefab = Scene::scene->CreateEmpty();
					prefab->AddComponent<Building>();
					BoxCollider* collider = prefab->AddComponent<BoxCollider>();
					collider->extents = { 1.5f, 0.5f, 0.4f };
					collider->obb = true;

					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = mesh;
					Renderer* renderer = child->AddComponent<Renderer>();
					for (auto& sm : mesh->DrawArgs)
						renderer->materials.push_back(mat);
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->layer = (int)RenderLayer::Opaque;
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
				break;
			case BuildingType::Spike:
				prefab = Scene::scene->CreateEmpty();
				prefab->AddComponent<Building>();
				{
					prefab = Scene::scene->CreateEmpty();
					prefab->AddComponent<Building>();
					{
						BoxCollider* collider = prefab->AddComponent<BoxCollider>();
						collider->extents = { 1.4f, 0.8f, 0.4f };
						collider->obb = true;
						mesh = ASSET MESH("SM_Spike");
						mat = ASSET MATERIAL("material_02");
						GameObject* child = prefab->AddChild();
						child->AddComponent<MeshFilter>()->mesh = mesh;
						Renderer* renderer = child->AddComponent<Renderer>();
						for (auto& sm : mesh->DrawArgs)
							renderer->materials.push_back(mat);
						child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
						child->layer = (int)RenderLayer::Opaque;
						child->transform->Scale({ scaleSize, scaleSize, scaleSize });
						child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
					}
					{
						mesh = ASSET MESH("SM_Fish");
						GameObject* child = prefab->AddChild();
						child->AddComponent<MeshFilter>()->mesh = mesh;
						Renderer* renderer = child->AddComponent<Renderer>();
						for (auto& sm : mesh->DrawArgs)
							renderer->materials.push_back(mat);
						child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
						child->layer = (int)RenderLayer::Opaque;
						child->transform->position = { -0.1f,0.75f,0.0f };
						child->transform->Scale({ scaleSize, scaleSize, scaleSize });
						child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
					}
				}
				break;
			case BuildingType::Cart_01:
				mesh = ASSET MESH("SM_Cart_Var01");
			case BuildingType::Cart_02:
				mesh = ASSET MESH("SM_Cart_Var02");
				mat = ASSET MATERIAL("material_01");
				{
					prefab = Scene::scene->CreateEmpty();
					prefab->AddComponent<Building>();
					BoxCollider* collider = prefab->AddComponent<BoxCollider>();
					collider->extents = { 1.7f, 0.5f, 1.0f };
					collider->obb = true;

					GameObject* child = prefab->AddChild();
					child->AddComponent<MeshFilter>()->mesh = mesh;
					Renderer* renderer = child->AddComponent<Renderer>();
					for (auto& sm : mesh->DrawArgs)
						renderer->materials.push_back(mat);
					child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
					child->layer = (int)RenderLayer::Opaque;
					child->transform->Scale({ scaleSize, scaleSize, scaleSize });
					child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				}
				break;
			default:
				prefab = Scene::scene->CreateEmpty();
				prefab->AddComponent<Building>();
				prefab->AddComponent<BoxCollider>()->extents = { colliderSize, colliderSize, colliderSize };

				GameObject* child = prefab->AddChild();
				child->AddComponent<MeshFilter>()->mesh = mesh;
				Renderer* renderer = child->AddComponent<Renderer>();
				for (auto& sm : mesh->DrawArgs)
					renderer->materials.push_back(mat);
				child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
				child->layer = (int)RenderLayer::Opaque;
				child->transform->Scale({ scaleSize, scaleSize, scaleSize });
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				break;
			}

			prefab->transform->position = { frontInform.xPos, frontInform.yPos, frontInform.zPos };
			prefab->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, frontInform.rotAngle);
			BuildManager::buildManager->buildings[frontInform] = prefab;
			prefab->GetComponent<Building>()->positionToAnimate = prefab->transform->position.y;
			prefab->transform->position.y -= prefab->GetComponent<BoxCollider>()->extents.y * 2 + 0.5f;
			if (HostNetwork::network != nullptr)
			{
				if (HostNetwork::network->isConnect)
				{	// Networking
					HostNetwork::network->send_construct_packet(frontInform);
				}
			}
			buildCommand.pop_front();
		}

		while (!destroyCommand.empty())
		{
			BuildingInform frontInform = destroyCommand.front();
			auto it = BuildManager::buildManager->buildings.find(frontInform);
			if (it != BuildManager::buildManager->buildings.end())
			{
				Scene::scene->PushDelete(it->second);
				BuildManager::buildManager->buildings.erase(it);
				if (HostNetwork::network != nullptr)
				{
					if (HostNetwork::network->isConnect)
					{	// Networking
						HostNetwork::network->send_destruct_packet(frontInform);
					}
				}
			}
			destroyCommand.pop_front();
		}
	}

	void BuildNewBuilding(BuildingInform b_inform)
	{
		buildCommand.emplace_back(b_inform);
	}

	void DestroyBuilding(BuildingInform b_inform)
	{
		destroyCommand.emplace_back(b_inform);
	}

	void DestroyAllBuilding()
	{
		for (auto& b : BuildManager::buildManager->buildings)
		{
			Scene::scene->PushDelete(b.second);
		}
		BuildManager::buildManager->buildings.clear();
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};