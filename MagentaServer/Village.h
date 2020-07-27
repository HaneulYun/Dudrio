#pragma once
#include "main.h"
// 랜드마크가 가지는 스크립트

class Village
{
public:
	//				Home			Sim
	unordered_map<BuildingInfo*, Sim*> simList;

	float delayTime = 0.f;	// 건설 쿨타임
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