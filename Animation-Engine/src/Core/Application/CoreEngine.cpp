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

	void CoreEngine::Initialize()
	{
		Camera::GetInstance()->Initialize();

		application->Initialize();

		sphere.Initialize();
	}

	void CoreEngine::Update()
	{
		application->PreUpdate();

		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableWireFrameMode(false);

		GridMesh gridMesh;

#if ANIM_DEBUG
		constexpr unsigned width = 20;
		constexpr unsigned height = 20;
		sphere.GetRadius() = 4.0f;
#else
		constexpr unsigned width = 50;
		constexpr unsigned height = 50;
		sphere.GetRadius() = 8.0f;
#endif

		static constexpr Math::Vec3F EXTERNAL_FORCE = { 0.0f, -5000.0f, 0.0f };

		constexpr float particleMass = 1.0f;

		Physics::Cloth cloth(width, height, particleMass);

		Physics::Wind wind;

#if ANIM_DEBUG
		GraphicsAPI::GetContext()->EnableVSync(true);
#endif

		while (isRunning && !window->WindowShouldClose())
		{
			GraphicsAPI::GetContext()->ClearColor();
			GraphicsAPI::GetContext()->ClearBuffer();

			Time::Update();

			application->Update();

			sphere.Render();

			while (Time::ShouldRunFixedUpdate())
			{
				if (startClothSimulation)
				{
					for (const auto& particle : cloth.GetParticles())
					{
						particle->AddForce(EXTERNAL_FORCE);
					}

					wind.Update(cloth.GetParticles());

					HandleCollisionWithSphere(sphere, cloth.GetParticles());

					cloth.FixedUpdate();
				}

				Time::ResetAccumulator();
			}

			cloth.Update();

			gridMesh.Update();

			ProcessInput(&cloth);

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

	void CoreEngine::ProcessInput(Physics::Cloth* cloth)
	{
		const auto camera = Camera::GetInstance();

		const auto glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());

		if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		    glfwSetWindowShouldClose(glfwWindow, true);

#ifdef ANIM_DEBUG
		static bool enableWireFrame = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_T) == GLFW_PRESS)
		{
			if (!enableWireFrame)
			{
				GraphicsAPI::GetContext()->ToggleWireFrameMode();
				enableWireFrame = true;
			}
		}
		else
		{
			enableWireFrame = false;
		}
#endif

		// Camera Movement
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
		if (glfwGetKey(glfwWindow, GLFW_KEY_I) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_KP_9) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::LOOK_UP);
		if (glfwGetKey(glfwWindow, GLFW_KEY_K) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_KP_3) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::LOOK_DOWN);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_0) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_J) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_LEFT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_L) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_RIGHT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_O) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_IN);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(glfwWindow, GLFW_KEY_P) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_OUT);

		// Reset Camera
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

		// Sphere Movement
		const float sphereMovementSpeed = 3.5f * Time::GetDeltaTime();

		if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
			sphere.GetCenter().z -= sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
			sphere.GetCenter().z += sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
			sphere.GetCenter().x += sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
			sphere.GetCenter().x -= sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
			sphere.GetCenter().y += sphereMovementSpeed;
		if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			sphere.GetCenter().y -= sphereMovementSpeed;

		// Cloth Controls
		static bool toggleIsStatic1 = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_1) == GLFW_PRESS)
		{
			if (!toggleIsStatic1)
			{
				cloth->ToggleIsStatic(0);
				toggleIsStatic1 = true;
			}
		}
		else
		{
			toggleIsStatic1 = false;
		}

		static bool toggleIsStatic2 = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS)
		{
			if (!toggleIsStatic2)
			{
				cloth->ToggleIsStatic(1);
				toggleIsStatic2 = true;
			}
		}
		else
		{
			toggleIsStatic2 = false;
		}

		static bool toggleIsStatic3 = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_3) == GLFW_PRESS)
		{
			if (!toggleIsStatic3)
			{
				cloth->ToggleIsStatic(2);
				toggleIsStatic3 = true;
			}
		}
		else
		{
			toggleIsStatic3 = false;
		}

		static bool toggleIsStatic4 = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_4) == GLFW_PRESS)
		{
			if (!toggleIsStatic4)
			{
				cloth->ToggleIsStatic(3);
				toggleIsStatic4 = true;
			}
		}
		else
		{
			toggleIsStatic4 = false;
		}

		static bool freeCloth = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_5) == GLFW_PRESS)
		{
			if (!freeCloth)
			{
				cloth->ToggleIsStatic(4);
				freeCloth = true;
			}
		}
		else
		{
			freeCloth = false;
		}

		// Simulation Controls
		static bool startClothSim = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			if (!startClothSim)
			{
				startClothSimulation = true;
				startClothSim = true;
			}
		}
		else
		{
			startClothSim = false;
		}

		static bool resetSimulation = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_R) == GLFW_PRESS)
		{
			if (!resetSimulation)
			{
				startClothSimulation = false;
				if (cloth)
				{
					cloth->ResetSimulation();
				}
				resetSimulation = true;
			}
		}
		else
		{
			resetSimulation = false;
		}
	}
}
