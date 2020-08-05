#pragma once
#include "Scene.h"

enum TAG { TAG_DEFAULT, TAG_BUILDING, TAG_PREVIEW };

class HostScene : public Scene
{
public:
	HostScene() : Scene() {}
	~HostScene() {}

	virtual void BuildObjects();
};