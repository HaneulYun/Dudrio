#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class CharacterMovingBehavior : public MonoBehavior<CharacterMovingBehavior>
{
private /*이 영역에 private 변수를 선언하세요.*/:


public  /*이 영역에 public 변수를 선언하세요.*/:
	Animator* anim{ nullptr };
	char name[MAX_ID_LEN];
	Vector3 velocity{ 0,0,0 };
	Vector3 accel{ 0,0,0 };

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
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		// 서버에서 받은 위치 부드럽게 보간하는 식 작성하기
		gameObject->transform->position += velocity * Time::deltaTime;
		gameObject->transform->position.y = heightmap->GetHeight(gameObject->transform->position.x, gameObject->transform->position.z);

		anim->SetFloat("VelocityZ", velocity.z);
		anim->SetFloat("VelocityX", velocity.x);
	}

	void move(Vector3 newPos)
	{
		gameObject->transform->position = newPos;
	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};