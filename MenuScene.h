#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene() : Scene() {}
	virtual ~MenuScene() {}

	virtual void BuildObjects();
};