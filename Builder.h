#pragma once
#include "CyanEngine\CyanEngine\framework.h"
#include <vector>

class Builder : public MonoBehavior<Builder>
{
private /*�� ������ private ������ �����ϼ���.*/:

	deque<BuildingInform> buildCommand;

public  /*�� ������ public ������ �����ϼ���.*/:
	static Builder* builder;

private:
	friend class GameObject;
	friend class MonoBehavior<Builder>;
	Builder() = default;
	Builder(Builder&) = default;

public:
	~Builder() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
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
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};