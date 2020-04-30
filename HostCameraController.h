#pragma once
#include "..\CyanEngine\framework.h"
#include "BulidManager.h"

class HostCameraController : public MonoBehavior<HostCameraController>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	float mTheta = 1.5f * XM_PI;
	float mPhi = 0.2f * XM_PI;
	float mRadius = 15.0f;
	Vector3 lookAtPos = { 0,0,0 };

	Vector3 lastMousePos;

public  /*이 영역에 public 변수를 선언하세요.*/:

private:
	friend class GameObject;
	friend class MonoBehavior<HostCameraController>;
	HostCameraController() = default;
	HostCameraController(HostCameraController&) = default;

public:
	~HostCameraController() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		float x = lookAtPos.x + mRadius * sinf(mPhi) * cosf(mTheta);
		float z = lookAtPos.z + mRadius * sinf(mPhi) * sinf(mTheta);
		float y = lookAtPos.y + mRadius * cosf(mPhi);

		gameObject->GetComponent<Transform>()->position = { x, y, z };
		gameObject->GetComponent<Transform>()->forward = (lookAtPos - Vector3(x, y, z)).Normalize();

		if (Input::GetMouseButtonDown(2))
		{
			lastMousePos = Input::mousePosition;
		}
		else if (Input::GetMouseButton(2) && BuildManager::buildManager->prefab == nullptr)
		{
			Vector3 currMousePos = Input::mousePosition;
			Vector3 delta = (currMousePos - lastMousePos) * 0.25f * (XM_PI / 180.0f);

			mTheta += delta.x;
			mPhi += delta.y;

			mPhi = FbxClamp(mPhi, 0.1f, PI - 0.1f);

			lastMousePos = currMousePos;
		}

		else if (Input::GetMouseWheelDelta())
		{
			float delta = Input::GetMouseWheelDelta();
			mRadius -= delta * 0.05f;
		}

		else if (Input::GetMouseButtonDown(0))
		{
			lastMousePos = Camera::main->ScreenToWorldPoint(Input::mousePosition);
		}
		else if (Input::GetMouseButton(0))
		{
			Vector3 currMousePos = Camera::main->ScreenToWorldPoint(Input::mousePosition);
			Vector3 dir = currMousePos - lastMousePos;

			lookAtPos.x -= dir.x * 50.0f;
			lookAtPos.z -= dir.z * 50.0f;

			lastMousePos = currMousePos;
		}

	}

	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};
