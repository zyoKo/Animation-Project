#include "Sandbox.h"

#include "Core/Application/CoreEngine.h"

namespace Sandbox
{
	void SandboxApp::Initialize()
	{
	}

	void SandboxApp::PreUpdate()
	{
	}

	void SandboxApp::Update()
	{
	}

	void SandboxApp::PostUpdate()
	{
	}

	void SandboxApp::Shutdown()
	{
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