#pragma once

#include <memory>
#include <string>

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Components/PointMesh.h"
#include "Core/Window/IWindow.h"

namespace AnimationEngine
{
	class IApplication;

	class Animator;

	class IAssetManager;
}

namespace AnimationEngine
{
	class CoreEngine
	{
	public:
		CoreEngine(const std::string& name = "Animation Window", uint32_t width = 1280, uint32_t height = 720);
		~CoreEngine();

		CoreEngine(const CoreEngine&) = delete;
		CoreEngine& operator=(const CoreEngine&) = delete;

		void SetApplication(const std::shared_ptr<IApplication>& app);

		void Initialize();

		void Update();

		bool Shutdown();

	private:
		std::shared_ptr<IApplication> application;

		std::shared_ptr<Animator> animator;

		std::unique_ptr<IWindow> window;

		IAssetManager* assetManager;

		bool running = true;

		bool enableModelMesh = false;

		AnimationStorage animationStorage;

		PointMesh* point;

		void ProcessInput();
	};
}
