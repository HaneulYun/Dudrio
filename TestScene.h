#pragma once
#include "Scene.h"

class TestScene : public Scene
{
public:
	TestScene() : Scene() {}
	~TestScene() {}

	virtual void BuildObjects();
};
