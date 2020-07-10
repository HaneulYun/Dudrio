#pragma once
#include "..\CyanEngine\framework.h"
#include "BulidManager.h"

class HostCameraController : public MonoBehavior<HostCameraController>
{
private /*�� ������ private ������ �����ϼ���.*/:
	float mTheta = 1.5f * XM_PI;
	float mPhi = 0.2f * XM_PI;
	float mRadius = 100.0f;
	Vector3 lookAtPos = { 0,0,0 };

	Vector3 lastMousePos;

public  /*�� ������ public ������ �����ϼ���.*/:
	TerrainData* heightmap;
private:
	friend class GameObject;
	friend class MonoBehavior<HostCameraController>;
	HostCameraController() = default;
	HostCameraController(HostCameraController&) = default;

public:
	~HostCameraController() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		lookAtPos = { (float)(heightmap->heightmapWidth / 2), 0, (float)(heightmap->heightmapHeight / 2) };
		float x = lookAtPos.x + mRadius * sinf(mPhi) * cosf(mTheta);
		float z = lookAtPos.z + mRadius * sinf(mPhi) * sinf(mTheta);
		float y = lookAtPos.y + mRadius * cosf(mPhi);

		gameObject->transform->position = { x, y, z };
		gameObject->transform->forward = (lookAtPos - Vector3(x, y, z)).Normalize();
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		float tTheta = mTheta, tPhi = mPhi, tRadius = mRadius;
		Vector3 tlookAtPos = lookAtPos;

		if (Input::GetMouseButtonDown(2))
		{
			lastMousePos = Input::mousePosition;
		}
		else if (Input::GetMouseButton(2))// && BuildManager::buildManager->prefab == nullptr)
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

		else if (Input::GetMouseButtonDown(0))
		{
			lastMousePos = Camera::main->ScreenToWorldPoint(Input::mousePosition);
		}
		else if (Input::GetMouseButton(0))
		{
			Vector3 currMousePos = Camera::main->ScreenToWorldPoint(Input::mousePosition);
			Vector3 dir = currMousePos - lastMousePos;

			tlookAtPos.x = lookAtPos.x - dir.x * mRadius;
			tlookAtPos.z = lookAtPos.z - dir.z * mRadius;

			lastMousePos = currMousePos;
		}

		float h = heightmap->GetHeight(gameObject->transform->position.x, gameObject->transform->position.z);
		
		if (tlookAtPos.y + tRadius * cosf(tPhi) - 1.0f < h)
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
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};
