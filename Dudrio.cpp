#include "pch.h"
#include "Dudrio.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	CyanFW* cyanFW = new CyanFW(1200, 800, L"YellowProject");

	SceneManager* sceneManager = SceneManager::Instance();

	sceneManager->scenes["MenuScene"] = new MenuScene();
	sceneManager->scenes["HostScene"] = new HostScene();
	sceneManager->scenes["HostMenuScene"] = new HostMenuScene();
	sceneManager->scenes["GuestMenuScene"] = new GuestMenuScene();
	sceneManager->scenes["ParticleScene"] = new ParticleScene();
	sceneManager->scenes["GuestScene"] = new GuestScene();

	Scene::scene = sceneManager->scene = sceneManager->scenes["MenuScene"];
	return CyanApp::Run(cyanFW, hInstance, nCmdShow);
}
