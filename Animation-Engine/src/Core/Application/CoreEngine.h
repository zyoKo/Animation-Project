#pragma once

#include <memory>
#include <string>

#include "Animation/Animation.h"
#include "Animation/Repository/AnimationStorage.h"
#include "Animation/ModelManager/ModelManager.h"
#include "Core/Window/IWindow.h"

namespace AnimationEngine
{
	class IApplication;

	class IKTarget;

	class CurveMesh;

	class IAnimator;

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

		CurveMesh* curveMesh;
		IKTarget* targetPoint;

		bool running = true;
		bool enableModelMesh = false;

		AnimationStorage animationStorage;
		ModelManager* modelManager;

		void ProcessInput() const;
	};
}
