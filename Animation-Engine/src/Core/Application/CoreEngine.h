#pragma once

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Core/Window/IWindow.h"
#include "Physics/Sphere.h"

namespace AnimationEngine::Physics
{
	class Cloth;
}

namespace AnimationEngine
{
	class IApplication;
	class IAnimator;
	class IAssetManager;
}

namespace AnimationEngine
{
	class CoreEngine
	{
	public:
		CoreEngine(std::string_view = WINDOW_TITLE, uint32_t width = WINDOW_WIDTH, uint32_t height = WINDOW_HEIGHT);

		~CoreEngine();

		CoreEngine(const CoreEngine&) = delete;
		CoreEngine& operator=(const CoreEngine&) = delete;

		CoreEngine(CoreEngine&&) = delete;
		CoreEngine& operator=(CoreEngine&&) = delete;

		void SetApplication(const std::shared_ptr<IApplication>& app);

		void Initialize();

		void Update();

		bool Shutdown();

	private:
		std::shared_ptr<IApplication> application;

		std::unique_ptr<IWindow> window;

		IAssetManager* assetManager;

		IAnimator* animator;

		bool isRunning;

		bool startClothSimulation;

		Physics::Sphere sphere;

		AnimationStorage animationStorage;

		void ProcessInput(Physics::Cloth* cloth);
	};
}
