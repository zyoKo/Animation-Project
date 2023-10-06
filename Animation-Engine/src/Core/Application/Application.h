#pragma once

#include <memory>
#include <string>

#include "Components/Camera/Camera.h"
#include "Core/Window/IWindow.h"

namespace Animator
{
	class AssetManager;
}

namespace Animator
{
	// --
	// Class Type: Singleton
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
		void ProcessCameraInput(Camera& camera);

		float deltaTime, lastFrame;

		static Application* instance;

		std::unique_ptr<IWindow> window;

		std::shared_ptr<AssetManager> assetManager;

		bool running = true;
	};
}
