#pragma once

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Core/Window/IWindow.h"
#include "Physics/Sphere.h"

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

		void Initialize() const;

		void Update();

		bool Shutdown();

	private:
		std::shared_ptr<IApplication> application;

		std::unique_ptr<IWindow> window;

		IAssetManager* assetManager;

		IAnimator* animator;

		bool isRunning = true;

		bool enableModelMesh = false;

		bool startClothSimulation = false;

		Physics::Sphere sphere{ { 10.0f, 15.0f, 10.0f}, 10.0f };

		AnimationStorage animationStorage;

		void ProcessInput();
	};
}
