#pragma once
#include "Scene.h"

class HostScene : public Scene
{
public:
	HostScene() : Scene() {}
	~HostScene() {}


	virtual void BuildObjects();

	GameObject* CreateTextButton();
	GameObject* CreateButtonList();
};