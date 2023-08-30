#pragma once

#include <memory>
#include <string>

#include "Core/Window/IWindow.h"

namespace Animator
{
	class Application
	{
	public:
		Application(const std::string& name = "Animation Window", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application() = default;

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		virtual void Initialize();

		virtual void Run();

		virtual void Render();

		virtual bool Shutdown();

	private:
		std::unique_ptr<IWindow> window;

		bool running = true;

		// Singleton Class
		static Application* instance;
	};
}
