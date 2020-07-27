#pragma once
#include "main.h"

class AIManager
{
private /*이 영역에 private 변수를 선언하세요.*/:

public  /*이 영역에 public 변수를 선언하세요.*/:
	static AIManager* aiManager;

public:
	AIManager();
	~AIManager() {}

	void AIUpdate();
};