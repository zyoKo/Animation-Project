#pragma once

#include <memory>
#include <string>

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Core/Window/IWindow.h"

namespace Animator
{
	class AnimatorR;

	class AssetManager;

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
		float deltaTime, lastFrame;

		std::shared_ptr<AnimatorR> animator;

		static Application* instance;

		std::unique_ptr<IWindow> window;

		std::shared_ptr<AssetManager> assetManager;

		bool running = true;

		bool enableModelMesh = false;

		AnimationStorage animationStorage;

		void ProcessInput(Camera& camera);

		void ChangeModelAndAnimation();
	};
}
