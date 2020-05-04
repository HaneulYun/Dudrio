#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include <vector>

class Builder : public MonoBehavior<Builder>
{
private /*이 영역에 private 변수를 선언하세요.*/:

	deque<BuildingInform> buildCommand;

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
		if (!buildCommand.empty())
		{
			auto newBuilding = Scene::scene->CreateEmpty();
			BuildingInform frontInform = buildCommand.front();
			switch (frontInform.buildingType)
			{
			case B_SPHERE:
			{
				newBuilding->GetComponent<Transform>()->Scale({ 1, 1, 1 });
				newBuilding->AddComponent<MeshFilter>()->mesh = Scene::scene->geometries["Sphere"].get();
				auto renderer = newBuilding->AddComponent<Renderer>();
				newBuilding->layer = (int)RenderLayer::Opaque;
				for (auto& sm : newBuilding->GetComponent<MeshFilter>()->mesh->DrawArgs)
					renderer->materials.push_back(10);
			}
			break;
			case B_CUBE:
			{
				newBuilding->GetComponent<Transform>()->Scale({ 5, 5, 5 });
				newBuilding->AddComponent<MeshFilter>()->mesh = Scene::scene->geometries["Cube"].get();
				auto renderer = newBuilding->AddComponent<Renderer>();
				newBuilding->layer = (int)RenderLayer::Opaque;
				for (auto& sm : newBuilding->GetComponent<MeshFilter>()->mesh->DrawArgs)
					renderer->materials.push_back(9);
			}
			break;
			default:
				break;
			}
			newBuilding->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
			newBuilding->transform->position = { frontInform.xPos, frontInform.yPos, frontInform.zPos };
			newBuilding->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, frontInform.rotAngle);
			
			buildCommand.pop_front();
		}
	}

	void BuildNewBuilding(BuildingInform b_inform)
	{
		buildCommand.emplace_back(b_inform);
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};