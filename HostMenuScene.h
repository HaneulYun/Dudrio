#pragma once
#include "Scene.h"

class HostMenuScene : public Scene
{
public:
	HostMenuScene() : Scene() {}
	~HostMenuScene() {}

	virtual void BuildObjects();
};