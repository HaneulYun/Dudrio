#pragma once
#include "..\CyanEngine\framework.h"

class CameraController : public MonoBehavior<CameraController>
{
private /*�� ������ private ������ �����ϼ���.*/:
	float mTheta = 1.5f * XM_PI;
	float mPhi = 0.2f * XM_PI;
	float mRadius = 5.0f;
	
	Vector3 lastMousePos;

	Vector3 lookAtPos = { 0,2,-3 };
public  /*�� ������ public ������ �����ϼ���.*/:

private:
	friend class GameObject;
	friend class MonoBehavior<CameraController>;
	CameraController() = default;
	CameraController(CameraController&) = default;

public:
	~CameraController() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		gameObject->transform->position = lookAtPos;
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		float x = lookAtPos.x + mRadius * sinf(mPhi) * cosf(mTheta);
		float z = lookAtPos.z + mRadius * sinf(mPhi) * sinf(mTheta);
		float y = lookAtPos.y + mRadius * cosf(mPhi);
		//
		//gameObject->transform->position = { x, y, z };
		//gameObject->transform->forward = (lookAtPos - Vector3(x, y, z)).Normalize();
		//Vector3 forward = (lookAtPos - Vector3(x, y, z)).Normalize();
		//Vector3 up = { 0,1,0 };
		//gameObject->parent->transform->forward = forward;
		//gameObject->parent->transform->right.xmf3 = NS_Vector3::CrossProduct(forward.xmf3, up.xmf3);

		if (Input::GetMouseButtonDown(2))
		{
			lastMousePos = Input::mousePosition;
		}
		else if (Input::GetMouseButton(2))
		{
			Vector3 currMousePos = Input::mousePosition;
			Vector3 delta = (currMousePos - lastMousePos) * 0.25f * (XM_PI / 180.0f);

			mTheta += delta.x;
			mPhi += delta.y;

			mPhi = FbxClamp(mPhi, 0.1f, PI - 0.1f);

			lastMousePos = currMousePos;
		}
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};
