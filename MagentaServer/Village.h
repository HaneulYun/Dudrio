#pragma once
#include "main.h"
// ���帶ũ�� ������ ��ũ��Ʈ

class Village
{
public:
	//				Home			Sim
	unordered_map<BuildingInfo*, Sim*> simList;

	float delayTime = 0.f;	// �Ǽ� ��Ÿ��
	bool autoDevelopment = false;

public:
	~Village() {}

	void OnAutoDevelopment()
	{
		autoDevelopment = true;
		delayTime = rand() % 10 + 10;
	}

	void OffAutoDevelopment()
	{
		autoDevelopment = false;
	}
};