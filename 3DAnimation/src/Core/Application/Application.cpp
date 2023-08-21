#include "Application.h"

#include <iostream>

namespace Animator
{
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		if (!instance)
		{
			std::cout << "Application already exists!" << std::endl;
		}
		instance = this;

		window = std::unique_ptr<IWindow>(IWindow::Create({ name, width, height }));

		// Bind Event Callback here
	}

	void Application::Initialize()
	{
	}

	void Application::Update()
	{
		while (running)
		{
			window->Update();
		}
	}

	void Application::Render()
	{
	}

	bool Application::Shutdown()
	{
		running = false;
		return true;
	}
}
