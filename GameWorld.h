#pragma once
#include "..\CyanEngine\framework.h"

class HostGameWorld;
class GuestGameWorld;

class GameWorld : public MonoBehavior<GameWorld>
{
public:
	enum BuildingType { Landmark, House, Theme, Landscape, Lighting, Fence, Prop, Count, None = -1 };
	enum TimeSpeed { X1 = 1, X2 = 2, X4 = 4, X8 = 8 };

private /*이 영역에 private 변수를 선언하세요.*/:

protected /*이 영역에 protected 변수를 선언하세요.*/:
	const int timeOfDay = 60 * 15;

public  /*이 영역에 public 변수를 선언하세요.*/:
	// 랜드마크에 따른 빌딩 목록
	std::map<GameObject*, std::map<BuildingType, std::vector<GameObject*>>> buildingList;
	// 전체 심 목록
	std::map<int, GameObject*>	simList;

	/*Time*/
	GameObject* sun;
	TimeSpeed		timeSpeed = X1;

	float			gameTime = 37.5f * 20;
	float			gameDeltaTime;

	/*money*/
	int				gameMoney;

private:
	friend class GameObject;
	friend class MonoBehavior<GameWorld>;
	friend class MonoBehavior<HostGameWorld, GameWorld>;
	friend class MonoBehavior<GuestGameWorld, GameWorld>;
	GameWorld() = default;
	GameWorld(GameWorld&) = default;

public:
	~GameWorld() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		sun->transform->Rotate({ 1, 0, 0 }, 90);
		sun->transform->Rotate({ 0, 1, 0 }, -90);
	}

	void Update(/*업데이트 코드를 작성하세요.*/) {}

	void calculateSunInfo()
	{
		Light* light = sun->GetComponent<Light>();
		light->Strength = { 0.9, 0.9, 0.9 };

		float sunRotAngle = 0.0f;
		if (gameTime < 6 * 37.5f) {
			sunRotAngle += 0.8 * gameTime;
		}
		else if (gameTime < 10 * 37.5f) {
			sunRotAngle += 0.8 * 6 * 37.5f;
			sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
			//light->Strength += (gameTime - (6 * 37.5f)) * 0.008f;
			//if (light->Strength.x > 0.9f)
			//	light->Strength = { 0.9, 0.9, 0.9 };
		}
		else if (gameTime < 21 * 37.5f) {
			sunRotAngle += 0.8 * 6 * 37.5f;
			sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
			//light->Strength = { 0.9, 0.9, 0.9 };
		}
		else {
			sunRotAngle += 0.8 * 6 * 37.5f;
			sunRotAngle += (gameTime - (6 * 37.5f)) * 0.26666667f;
			light->Strength = { 0.9, 0.9, 0.9 };
			//light->Strength -= (gameTime - (21 * 37.5f)) * 0.008f;
			//if (light->Strength.x < 0.0f)
			//	light->Strength = { 0,0,0 };
		}

		sun->transform->forward = Vector3(1, 0, 0).TransformNormal(Matrix4x4::RotationZ(XMConvertToRadians(sunRotAngle)));
	}

	std::wstring convertTimeToText()
	{
		int hour;
		int minute;

		hour = std::floor(gameTime / 37.5f);
		minute = std::floor((gameTime - (hour * 37.5f)) / 0.625f);

		std::wstring text = std::to_wstring(hour) + L":" + std::to_wstring(minute);
		return text;
	}
};