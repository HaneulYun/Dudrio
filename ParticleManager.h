#pragma once
#include "..\CyanEngine\framework.h"

class ParticleManager : public MonoBehavior<ParticleManager>
{
private /*�� ������ private ������ �����ϼ���.*/:

public  /*�� ������ public ������ �����ϼ���.*/:
	ParticleSystem* particleSystem{ nullptr };
	
	float effectTime{ 0.0f };

private:
	friend class GameObject;
	friend class MonoBehavior<ParticleManager>;
	ParticleManager() = default;
	ParticleManager(ParticleManager&) = default;

public:
	~ParticleManager() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		particleSystem = gameObject->GetComponent<ParticleSystem>();
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		if (effectTime > 0.0f)
			effectTime -= Time::deltaTime;
		else
			particleSystem->enabled = false;
	}

	void Enable(float time, Vector3 pos)
	{
		effectTime = time;
		gameObject->transform->position = pos;
		particleSystem->enabled = true;
	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};