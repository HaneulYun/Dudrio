#include "pch.h"
#include "Dudrio.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	CyanFW* cyanFW = new CyanFW(800, 600, L"YellowProject");

	SceneManager* sceneManager = SceneManager::Instance();
	sceneManager->scenes["MenuScene"] = new MenuScene();
	Scene::scene = sceneManager->scene = sceneManager->scenes["MenuScene"];

	return CyanApp::Run(cyanFW, hInstance, nCmdShow);
}
