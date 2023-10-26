#include "Sandbox.h"

#include <iostream>

#include "Core/Application/CoreEngine.h"

namespace Sandbox
{
	void SandboxApp::Initialize()
	{
		std::cout << "Hi from sandbox\n";
	}

	void SandboxApp::Update()
	{
	}

	bool SandboxApp::Shutdown()
	{
		return true;
	}
}

std::shared_ptr<AnimationEngine::IApplication> AnimationEngine::CreateApplication()
{
	return std::make_shared<Sandbox::SandboxApp>();
}

int main()
{
	AnimationEngine::CoreEngine app;

	app.SetApplication(AnimationEngine::CreateApplication());

	app.Initialize();

	app.Update();

	app.Shutdown();

	return 0;
}