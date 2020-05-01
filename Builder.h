#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class Builder : public MonoBehavior<Builder>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	GameObject* prefab{ nullptr };
	char buildType;
	Vector3 pos;
	float rotAngle;

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
		if (prefab)
		{
			switch (buildType)
			{
			case B_SPHERE:
			{
				prefab->GetComponent<Transform>()->Scale({ 1, 1, 1 });
				prefab->AddComponent<MeshFilter>()->mesh = Scene::scene->geometries["Sphere"].get();
				auto renderer = prefab->AddComponent<Renderer>();
				prefab->layer = (int)RenderLayer::Opaque;
				for (auto& sm : prefab->GetComponent<MeshFilter>()->mesh->DrawArgs)
					renderer->materials.push_back(10);
			}
			break;
			case B_CUBE:
			{
				prefab->GetComponent<Transform>()->Scale({ 5, 5, 5 });
				prefab->AddComponent<MeshFilter>()->mesh = Scene::scene->geometries["Cube"].get();
				auto renderer = prefab->AddComponent<Renderer>();
				prefab->layer = (int)RenderLayer::Opaque;
				for (auto& sm : prefab->GetComponent<MeshFilter>()->mesh->DrawArgs)
					renderer->materials.push_back(9);
			}
			break;
			default:
				break;
			}
			prefab->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			prefab->transform->position = pos;
			prefab->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, rotAngle);
			auto building = Scene::scene->Duplicate(prefab);

			DeletePrefab();
		}
	}

	void BuildNewBuilding(char b_type, Vector3 b_pos, float b_angle)
	{
		prefab = Scene::scene->CreateEmptyPrefab();
		buildType = b_type;
		pos = b_pos;
		rotAngle = b_angle;
	}

	void DeletePrefab()
	{
		Scene::scene->PushDelete(prefab);
		prefab = nullptr;
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};