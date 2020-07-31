#pragma once
#include "Scene.h"

class GuestMenuScene : public Scene
{
public:
	GuestMenuScene() : Scene() {}
	~GuestMenuScene() {}

	virtual void BuildObjects();
};