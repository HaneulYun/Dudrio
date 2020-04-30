#include "pch.h"
#include "Dudrio.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	CyanFW* cyanFW = new CyanFW(800, 600, L"YellowProject");

	SceneManager* sceneManager = SceneManager::Instance();
	sceneManager->scenes["MenuScene"] = new MenuScene();
	sceneManager->scenes["TerrainScene"] = new TerrainScene();
	sceneManager->scenes["MaterialScene"] = new MaterialScene();
	sceneManager->scenes["AnimationScene"] = new AnimationScene();
	sceneManager->scenes["ParticleScene"] = new ParticleScene();
	sceneManager->scenes["SampleScene"] = new SampleScene();
	//sceneManager->scenes["TerrainScene"] = new TerrainScene();
	Scene::scene = sceneManager->scene = sceneManager->scenes["MenuScene"];
	return CyanApp::Run(cyanFW, hInstance, nCmdShow);
}
