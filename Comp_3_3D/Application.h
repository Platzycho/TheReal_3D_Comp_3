#pragma once

#include <Window.h>
#include <Scene.h>
#include <memory>

class Application
{
public:
	Application(const Application&) = delete;
	Application(Application&&) = delete;

	static Application* Get();

	int Run();

private:
	Application() = default;

	void Init();
	void InitializeGLFW();
	void LoadContent();

	// m = member
	Scene mScene{ "Scene" };
	Window mWindow{ "LearnOpenGL", &mScene, 1280, 720 };
};

