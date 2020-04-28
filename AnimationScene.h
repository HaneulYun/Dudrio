#pragma once
#include "Scene.h"

class AnimationScene : public Scene
{
public:
	AnimationScene() : Scene() {}
	~AnimationScene() {}

	virtual void BuildObjects();
};
