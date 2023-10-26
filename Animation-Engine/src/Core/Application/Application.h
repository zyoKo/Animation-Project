#pragma once

#include <memory>
#include <string>

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Core/Window/IWindow.h"

namespace AnimationEngine
{
	class Animator;

	class IAssetManager;
}

namespace AnimationEngine
{
	class Application
	{
	public:
		Application(const std::string& name = "Animation Window", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		virtual void Initialize();

		virtual void Run();

		virtual void Render();

		virtual bool Shutdown();

	private:
		static Application* instance;

		//float deltaTime, lastFrame;

		std::shared_ptr<Animator> animator;

		std::unique_ptr<IWindow> window;

		IAssetManager* assetManager;

		bool running = true;

		bool enableModelMesh = false;

		AnimationStorage animationStorage;

		void ProcessInput();
	};
}
