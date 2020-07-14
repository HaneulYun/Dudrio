#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class CharacterMovingBehavior : public MonoBehavior<CharacterMovingBehavior>
{
private /*이 영역에 private 변수를 선언하세요.*/:


public  /*이 영역에 public 변수를 선언하세요.*/:
	Animator* anim{ nullptr };
	Vector3 animVel{ 0,0,0 };
	char name[MAX_ID_LEN];

	queue<pair<Vector3, float>> moveQueue;
	Vector3 destPos{ 0,0,0 };
	Vector3 curPos{ 0,0,0 };
	float destAngle;
	float curAngle;
	bool moving{ false };

	float dx, dz, dr;

	TerrainData* heightmap = NULL;

private:
	friend class GameObject;
	friend class MonoBehavior<CharacterMovingBehavior>;
	CharacterMovingBehavior() = default;
	CharacterMovingBehavior(CharacterMovingBehavior&) = default;

public:
	~CharacterMovingBehavior() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		destPos = { 0,0,0 };
		destAngle = 0.0f;
		curPos = { 0,0,0 };
		curAngle = 0.0f;
		animVel = { 0,0,0 };

		dx = 0.0f;
		dz = 0.0f;
		dr = 0.0f;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		if (!moveQueue.empty() && !moving) {
			destPos = moveQueue.front().first - gameObject->transform->position;
			destPos.y = 0;
			curPos = { 0,0,0 };
			destAngle = moveQueue.front().second;
			curAngle = 0.0f;
			animVel = { 0,0,0 };
			moveQueue.pop();
			moving = true;
		}

		if (moving) {
			if (curPos.Length() < destPos.Length()) {
				dx = destPos.x * Time::deltaTime * 3;
				dz = destPos.z * Time::deltaTime * 3;

				if ((curPos + Vector3{ dx, 0, dz }).Length() > destPos.Length()) {
					dx = destPos.x - curPos.x;
					dz = destPos.z - curPos.z;
				}

				gameObject->transform->position += {dx, 0, dz};
				curPos += {dx, 0, dz};
			}
			if (fabs(curAngle) < fabs(destAngle)) {
				dr = destAngle * Time::deltaTime * 3;

				if (fabs(curAngle + dr) > fabs(destAngle))
					dr = destAngle - curAngle;

				gameObject->transform->Rotate(Vector3{ 0,1,0 }, dr);
				curAngle += dr;
			}
			if (curPos.Length() >= destPos.Length() && fabs(curAngle) >= fabs(destAngle)) {

				moving = false;
			}
		}
		else {
			dx = 0.0f;
			dz = 0.0f;
		}

		Vector3 characterForward = gameObject->transform->forward;
		characterForward.y = 0;
		characterForward = characterForward.Normalized();
		Vector3 forward = { 0,0,1 };
		float angle = NS_Vector3::DotProduct(forward.xmf3, characterForward.xmf3);
		XMFLOAT3 dir = NS_Vector3::CrossProduct(forward.xmf3, characterForward.xmf3);
		angle = acos(angle);
		angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;
		animVel = { dx * cos(angle) - dz * sin(angle), 0,
			dx * sin(angle) + dz * cos(angle) };
		
		gameObject->transform->position.y = heightmap->GetHeight(gameObject->transform->position.x, gameObject->transform->position.z);
		anim->SetFloat("VelocityX", animVel.x * 1000.f);
		anim->SetFloat("VelocityZ", animVel.z * 1000.f);

	}

	void move(float xPos, float zPos)
	{
		gameObject->transform->position.x = xPos;
		gameObject->transform->position.y = heightmap->GetHeight(xPos, zPos);
		gameObject->transform->position.z = zPos;
	}

	void add_move_queue(Vector3 destPos, float rotAngle)
	{
		moveQueue.push(make_pair(destPos, rotAngle));
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};