#pragma once
#include "Scene.h"

class GuestScene: public Scene
{
public:
	GuestScene() : Scene() {}
	~GuestScene() {}

	virtual void BuildObjects();
};