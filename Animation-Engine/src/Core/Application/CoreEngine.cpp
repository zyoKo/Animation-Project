#include <AnimationPch.h>

#include "CoreEngine.h"

#include <GLFW/glfw3.h>

#include "Interface/IApplication.h"
#include "Core/Logger/Log.h"
#include "Core/Utilities/Time.h"
#include "Graphics/GraphicsAPI.h"
#include "AssetManager/AssetManager.h"
#include "Components/GridMesh.h"
#include "Components/Camera/Camera.h"
#include "Animation/Animator.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/ServiceLocators/Animation/AnimatorLocator.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Physics/Cloth.h"
#include "Physics/Sphere.h"
#include "Physics/Wind.h"
#include "Physics/Particle/Particle.h"

namespace AnimationEngine
{
	CoreEngine::CoreEngine(std::string_view name, uint32_t width, uint32_t height)
		:	assetManager(new AssetManager()),
			animator(new Animator())
	{
		Log::Initialize();

		window = std::unique_ptr<IWindow>(IWindow::Create({ name, width, height }));

		// Bind Event Callback here

		AssetManagerLocator::Initialize();
		AnimatorLocator::Initialize();

		AssetManagerLocator::Provide(assetManager);
		AnimatorLocator::Provide(animator);
		AnimationStorageLocator::Provide(&animationStorage);
	}

	CoreEngine::~CoreEngine()
	{
		delete assetManager;
		assetManager = nullptr;

		delete animator;
		animator = nullptr;
	}

	void CoreEngine::SetApplication(const std::shared_ptr<IApplication>& app)
	{
		this->application = app;
	}

	void CoreEngine::Initialize() const
	{
		Camera::GetInstance()->Initialize();

		application->Initialize();
	}

	void CoreEngine::Update()
	{
		application->PreUpdate();

		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableWireFrameMode(false);

		GridMesh gridMesh;

		constexpr unsigned width = 50;
		constexpr unsigned height = 50;
		constexpr float particleMass = 0.08f;

		Physics::Cloth cloth(width, height, particleMass);

		Physics::Wind wind;

		while (isRunning && !window->WindowShouldClose())
		{
			GraphicsAPI::GetContext()->ClearColor();
			GraphicsAPI::GetContext()->ClearBuffer();

			Time::Update();

			application->Update();

			if (startClothSimulation)
			{
				for (const auto& particle : cloth.GetParticles())
				{
					particle->AddForce({ 0.0f, -1000.0f, 0.0f });
				}

				wind.Update(cloth.GetParticles());

				HandleCollisionWithSphere(sphere, cloth.GetParticles());

				cloth.Update();
			}

			gridMesh.Update();

			ProcessInput();

			window->Update();
		}

		assetManager->ClearStores();

		application->PostUpdate();

		cloth.Clear();
	}

	bool CoreEngine::Shutdown()
	{
		application->Shutdown();

		isRunning = false;
		return true;
	}

	void CoreEngine::ProcessInput()
	{
		const auto camera = Camera::GetInstance();

		const auto glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());

		if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		    glfwSetWindowShouldClose(glfwWindow, true);

		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_8) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::FORWARD);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_2) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::BACKWARD);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_4) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::LEFT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_6) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::RIGHT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::DOWN);
		if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::UP);
		if (glfwGetKey(glfwWindow, GLFW_KEY_I) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::LOOK_UP);
		if (glfwGetKey(glfwWindow, GLFW_KEY_K) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::LOOK_DOWN);

		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS  || glfwGetKey(glfwWindow, GLFW_KEY_J) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_LEFT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_0) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_L) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_RIGHT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_O) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_IN);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_P) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_OUT);

		const float sphereMovementSpeed = 2.0f * Time::GetDeltaTime();

		if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
			sphere.center.z -= sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
			sphere.center.z += sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
			sphere.center.x += sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
			sphere.center.x -= sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
			sphere.center.y += sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			sphere.center.y -= sphereMovementSpeed;

		static bool isCameraResetKeyPressed = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_5) == GLFW_PRESS)
		{
			if (!isCameraResetKeyPressed)
			{
				camera->Reset();
				isCameraResetKeyPressed = true;
			}
		}
		else
		{
			isCameraResetKeyPressed = false;
		}

		static bool startClothSim = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			if (!startClothSim)
			{
				startClothSimulation = true;
				startClothSim = true;
			}
		}
	}
}
