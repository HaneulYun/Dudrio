#pragma once
#include "..\CyanEngine\framework.h"

class HostCameraController : public MonoBehavior<HostCameraController>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	float mTheta = 1.5f * XM_PI;
	float mPhi = 0.2f * XM_PI;
	float mRadius = 100.0f;
	Vector3 lookAtPos = { 0,0,0 };

	Vector3 lastMousePos;

public  /*이 영역에 public 변수를 선언하세요.*/:
	TerrainData* terrainData;
private:
	friend class GameObject;
	friend class MonoBehavior<HostCameraController>;
	HostCameraController() = default;
	HostCameraController(HostCameraController&) = default;

public:
	~HostCameraController() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		Vector2 posXZ{ (float)terrainData->heightmapWidth * 0.5f, (float)terrainData->heightmapHeight * 0.5f };
		lookAtPos = { posXZ.x, terrainData->GetHeight(posXZ.x,posXZ.y), posXZ.y };
		float x = lookAtPos.x + mRadius * sinf(mPhi) * cosf(mTheta);
		float z = lookAtPos.z + mRadius * sinf(mPhi) * sinf(mTheta);
		float y = lookAtPos.y + mRadius * cosf(mPhi);

		gameObject->transform->position = { x, y, z };
		gameObject->transform->forward = (lookAtPos - Vector3(x, y, z)).Normalize();
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		float tTheta = mTheta, tPhi = mPhi, tRadius = mRadius;
		Vector3 tlookAtPos = lookAtPos;

		Vector3 forward = Vector3::Normalize(gameObject->transform->forward);
		Vector3 right = Vector3(forward.z, 0, -forward.x);

		if (Input::GetKey(KeyCode::W))
		{
			tlookAtPos += forward * mRadius * Time::deltaTime;
		}
		if (Input::GetKey(KeyCode::A))
		{
			tlookAtPos -= right * mRadius * Time::deltaTime;
		}
		if (Input::GetKey(KeyCode::S))
		{
			tlookAtPos -= forward * mRadius * Time::deltaTime;
		}
		if (Input::GetKey(KeyCode::D))
		{
			tlookAtPos += right * mRadius * Time::deltaTime;
		}
		tlookAtPos.y = terrainData->GetHeight(tlookAtPos.x, tlookAtPos.z);


		if (Input::GetMouseButtonDown(1))
		{
			lastMousePos = Input::mousePosition;
		}
		else if (Input::GetMouseButton(1))
		{
			Vector3 currMousePos = Input::mousePosition;
			Vector3 delta = (currMousePos - lastMousePos) * 0.25f * (XM_PI / 180.0f);

			tTheta = mTheta + delta.x;
			tPhi = mPhi + delta.y;

			tPhi = FbxClamp(tPhi, 0.1f, PI - 0.1f);

			lastMousePos = currMousePos;
		}
		else if (Input::GetMouseWheelDelta())
		{
			tRadius = mRadius - Input::GetMouseWheelDelta() * 0.05f;
		}

		float terrainHeight = terrainData->GetHeight(tlookAtPos.x + tRadius * sinf(tPhi) * cosf(tTheta), tlookAtPos.z + tRadius * sinf(tPhi) * sinf(tTheta));
		
		if (tlookAtPos.y + tRadius * cosf(tPhi) < terrainHeight)
			return;

		lookAtPos = tlookAtPos;
		mRadius = tRadius;
		mTheta = tTheta; 
		mPhi = tPhi;

		float x = lookAtPos.x + mRadius * sinf(mPhi) * cosf(mTheta);
		float z = lookAtPos.z + mRadius * sinf(mPhi) * sinf(mTheta);
		float y = lookAtPos.y + mRadius * cosf(mPhi);

		gameObject->transform->position = { x, y, z };
		gameObject->transform->forward = (lookAtPos - Vector3(x, y, z)).Normalize();

	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};