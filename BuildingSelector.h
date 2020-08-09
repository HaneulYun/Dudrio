#pragma once
#include "..\CyanEngine\framework.h"

class BuildingSelector : public MonoBehavior<BuildingSelector>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	int maxPage{ 0 };
	int page{ 0 };

public  /*이 영역에 public 변수를 선언하세요.*/:
	BuildingBuilder::BuildingType type{ BuildingBuilder::BuildingType::None };
	BuildingBuilder* buildingBuilder{ nullptr };

	GameObject* buildingButtons[10]{};

	std::vector<GameObject*> previewer;

protected:
	friend class GameObject;
	friend class MonoBehavior<BuildingSelector>;
	BuildingSelector() = default;
	BuildingSelector(BuildingSelector&) = default;

public:
	~BuildingSelector() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		maxPage = buildingBuilder->getBuildingCount(type) / 10;
		void(*events[10])(void*)
		{
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(0); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(1); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(2); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(3); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(4); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(5); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(6); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(7); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(8); },
			[](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->invokeBuilder(9); }
		};

		for (int i = 0; i < 10; ++i)
		{
			auto buildingButton = gameObject->AddChildUI(Scene::scene->CreateImagePrefab());
			addChildAsTextButton(L"", 0.1 * i + 0.05, 0.5, 40, 40, buildingButton)->AddEvent(events[i], this);
			buildingButtons[i] = buildingButton;
			buildingButton->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("buiding_background");
		}
		addChildAsTextButton(L"◀", -0.025, 0.5, 20, 20)->AddEvent([](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->prevPage(); }, this);
		addChildAsTextButton(L"▶", 1.025, 0.5, 20, 20)->AddEvent([](void* ptr) { reinterpret_cast<BuildingSelector*>(ptr)->nextPage(); }, this);

		gameObject->SetActive(false);
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
	}

	void OnDisable()
	{
		releasePreview();
	}

	void releasePreview()
	{
		for (auto gameObject : previewer)
			Scene::scene->PushDelete(gameObject);
		previewer.clear();
	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
	void invokeBuilder(int index)
	{
		buildingBuilder->enterBuildMode(type, page * 10 + index);
	}

	void prevPage()
	{
		if (page > 0)
		{
			--page;
			releasePreview();
			setBuildingButtonName();
		}
	}

	void nextPage()
	{
		if (page < maxPage)
		{
			++page;
			releasePreview();
			setBuildingButtonName();
		}
	}

	void setBuildingButtonName()
	{
		for (int i = 0; i < 10; ++i)
		{
			wstring text = buildingBuilder->getBuildingName(type, page * 10 + i);

			auto buildingData = buildingBuilder->getBuildingData(type, page * 10 + i);
			if (buildingData.mesh)
			{
				auto gameObject = Scene::scene->CreateEmpty();
				{
					auto bx = buildingData.mesh->Bounds;
					Matrix4x4 projection = Matrix4x4::MatrixOrthographicOffCenterLH(
						bx.Center.x - bx.Extents.x * 1.25, bx.Center.x + bx.Extents.x * 1.25,
						bx.Center.z - bx.Extents.z * 1.25, bx.Center.z + bx.Extents.z * 1.25,
						-10, 10);

					Vector3 pos = Vector3(1, 1, -1).Normalize();
					Vector3 lookAt = Vector3(0);
					Vector3 up{ 0, 1, 0 };
					Matrix4x4 view = Matrix4x4::MatrixLookAtLH(pos, lookAt, up);

					gameObject->transform->Scale({ 1, 1, 1 });
					gameObject->transform->Rotate({ 1, 0, 0 }, -90);
					//gameObject->transform->position = { 0, 0, 0 };
					gameObject->transform->localToWorldMatrix = gameObject->transform->localToWorldMatrix * view * projection;
					gameObject->AddComponent<MeshFilter>()->mesh = buildingData.mesh;
					gameObject->AddComponent<Renderer>()->materials.push_back(buildingData.material);
					gameObject->layer = (int)RenderLayer::OnUI;

					auto onUI = gameObject->AddComponent<OnUI>();

					auto mat = RectTransform::Transform(buildingButtons[i]->GetMatrix());
					Vector3 leftTop{ mat._41, mat._22 + mat._42, 0 };
					Vector3 rightBottom{ mat._11 + mat._41, mat._42, 0 };

					UINT width = CyanFW::Instance()->GetWidth();
					UINT height = CyanFW::Instance()->GetHeight();

					onUI->leftTop.x = (leftTop.x / 2.0f + 0.5f) * width;
					onUI->leftTop.y = (leftTop.y / -2.0f + 0.5f) * height;
					onUI->rightBottom.x = (rightBottom.x / 2.0f + 0.5f) * width - onUI->leftTop.x;
					onUI->rightBottom.y = (rightBottom.y / -2.0f + 0.5f) * height - onUI->leftTop.y;
				}

				buildingButtons[i]->GetComponent<Text>()->text = L"";
				previewer.push_back(gameObject);
			}
			else
			{
				buildingButtons[i]->GetComponent<Text>()->text = text;
			}
		}
	}

	Button* addChildAsTextButton(wstring str, float u, float v, float w, float h, GameObject* _child = nullptr)
	{
		auto child = _child ? _child : gameObject->AddChildUI(Scene::scene->CreateImagePrefab());
		auto rt = child->GetComponent<RectTransform>();
		rt->setAnchorAndPivot(u, v);
		rt->pivot = { 0.5, 0.5 };
		rt->setPosAndSize(0, 0, w, h);

		auto text = child->AddComponent<Text>();
		text->text = str;
		text->fontSize = 10;
		text->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		text->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;

		if (wcscmp(str.c_str(), L"◀") == 0)
		{
			child->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("prev_page");
			text->text = L"";
		}
		else if (wcscmp(str.c_str(), L"▶") == 0)
		{
			child->GetComponent<Renderer>()->materials[0] = ASSET MATERIAL("next_page");
			text->text = L"";
		}

		return child->AddComponent<Button>();
	}
};