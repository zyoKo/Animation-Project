#include <AnimationPch.h>

#include "CoreEngine.h"

#include <GLFW/glfw3.h>
#include <execution>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Interface/IApplication.h"
#include "Core/Logger/Log.h"
#include "Core/Utilities/Time.h"
#include "Graphics/GraphicsAPI.h"
#include "AssetManager/AssetManager.h"
#include "Components/CurveMesh.h"
#include "Components/GridMesh.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/Constants/CameraConstants.h"
#include "Animation/Animator.h"
#include "Animation/IK/IKManager.h"
#include "Animation/Model.h"
#include "Components/IKTarget.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/ServiceLocators/Animation/AnimatorLocator.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Math/Spline/Constants/SplineConstants.h"

namespace AnimationEngine
{
	CoreEngine::CoreEngine(const std::string& name, uint32_t width, uint32_t height)
		:	assetManager(new AssetManager()),
			animator(new Animator()),
			curveMesh(nullptr),
			targetPoint(nullptr),
			modelManager(nullptr)
	{
		Log::Initialize();

		window = std::unique_ptr<IWindow>(IWindow::Create({name, width, height}));

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

		delete modelManager;
		modelManager = nullptr;

		delete curveMesh;
		curveMesh = nullptr;

		delete targetPoint;
		targetPoint = nullptr;
	}

	void CoreEngine::SetApplication(const std::shared_ptr<IApplication>& app)
	{
		this->application = app;
	}

	void CoreEngine::Initialize() const
	{
		Camera::GetInstance()->Initialize();

		application->Initialize();

		auto* animationStorage = AnimationStorageLocator::GetAnimationStorage();

		const std::string dreyarDiffuseTextureFile = "./assets/dreyar/textures/Dreyar_diffuse.png";
		const auto dreyarTextureDiffuse = assetManager->CreateTexture(dreyarDiffuseTextureFile);
		dreyarTextureDiffuse.lock()->SetTextureName("texture_diffuse1");

		const std::string gridTextureFile = "./assets/grid.png";
		const auto gridTexture = assetManager->CreateTexture(gridTextureFile);
		gridTexture.lock()->SetTextureName("gridTexture");

		const std::string vertexShaderFile = "./assets/shaders/anim_model.vert";
		const std::string fragmentShaderFile = "./assets/shaders/anim_model.frag";
		assetManager->CreateShader("AnimationShader", vertexShaderFile, fragmentShaderFile);

		const std::string debugVertexShaderFile = "./assets/shaders/debug_anim_model.vert";
		const std::string debugFragShaderFile = "./assets/shaders/debug_anim_model.frag";
		assetManager->CreateShader("DebugAnimationShader", debugVertexShaderFile, debugFragShaderFile);

		const std::string gridVertexShaderFile = "./assets/shaders/inf_grid.vert";
		const std::string gridFragmentShaderFile = "./assets/shaders/inf_grid.frag";
		assetManager->CreateShader("GridShader", gridVertexShaderFile, gridFragmentShaderFile);

		// Adding Model And Animation to Storage
		//const std::string dreyar1ColladaFile = "./assets/dreyar/Capoeira.dae";
		const std::string dreyar2ColladaFile = "./assets/dreyar/Walking.dae";
		const std::string dreyar3ColladaFile = "./assets/dreyar/Running.dae";
		//animationStorage.AddAssetToStorage(dreyar1ColladaFile, dreyarTextureDiffuse);
		animationStorage->AddAssetToStorage(dreyar2ColladaFile, dreyarTextureDiffuse.lock());
		animationStorage->AddAssetToStorage(dreyar3ColladaFile, dreyarTextureDiffuse.lock());

		// Spline
		const std::string splineVertShader = "./assets/shaders/spline.vert";
		const std::string splineFragShader = "./assets/shaders/spline.frag";
		assetManager->CreateShader("SplineShader", splineVertShader, splineFragShader);

		const std::string controlPointsVertShader = "./assets/shaders/spline_cp.vert";
		const std::string controlPointsFragShader = "./assets/shaders/spline_cp.frag";
		assetManager->CreateShader("ControlPointShader", controlPointsVertShader, controlPointsFragShader);
	}

	void CoreEngine::Update()
	{
		application->PreUpdate();

		auto animationShader = assetManager->RetrieveShaderFromStorage("AnimationShader");
		auto debugShader = assetManager->RetrieveShaderFromStorage("DebugAnimationShader");
		auto gridShader = assetManager->RetrieveShaderFromStorage("GridShader");
		auto splineShader = assetManager->RetrieveShaderFromStorage("SplineShader");
		auto controlPointShader = assetManager->RetrieveShaderFromStorage("ControlPointShader");

		auto textureDiffuse = assetManager->RetrieveTextureFromStorage("Dreyar_diffuse");
		auto gridTexture = assetManager->RetrieveTextureFromStorage("grid");

		animator->ChangeAnimation(animationStorage.GetAnimationForCurrentlyBoundIndex());

		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableWireFrameMode(false);

		auto camera = Camera::GetInstance();

		DebugMesh debugMesh(debugShader.lock());
		GridMesh gridMesh;
		gridMesh.SetGridTexture(gridTexture.lock());

		curveMesh = new CurveMesh();
		curveMesh->SetSplineShader(splineShader.lock());
		curveMesh->SetControlPointsShader(controlPointShader.lock());

		modelManager = new ModelManager();
		modelManager->SetSpline(curveMesh->GetSpline());

		IKManager iKManager;
		iKManager.Initialize();

		targetPoint = new IKTarget(curveMesh, &iKManager);

		if (auto* realAnimator = dynamic_cast<Animator*>(animator))
		{
			realAnimator->SetIKManager(&iKManager);
		}

		// Initialize to go to point
		curveMesh->CreateNewSplinePath(targetPoint->GetControlPoints());
		modelManager->Reset();

		while (running && !window->WindowShouldClose())
		{
			GraphicsAPI::GetContext()->ClearColor();
			GraphicsAPI::GetContext()->ClearBuffer();

			Time::Update();

			application->Update();

			// TODO: Build a better camera system
			// camera/view transformation
			glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)window->GetWidth() / (float)window->GetHeight(), CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
			glm::mat4 view = camera->GetViewMatrix();
			glm::mat4 model = glm::mat4(1.0f);

			ProcessInput();

			targetPoint->Update();	// IK Target Point Draw
			// Set target for TargetFinder and the target to solve FABRIK on
			iKManager.SetTargetFinderTargetPosition(targetPoint->GetTargetLocation());
			iKManager.SetTargetPosition(targetPoint->GetPseudoTargetLocation());

			animator->UpdateAnimation();
			//animator->ResetAnimation();

			iKManager.Update();

			// TODO: Fix these hacks
			debugMesh.OverwriteJointsPosition(animator->GetJointPositions());
			animator->ClearJoints();
			static bool firstRun = true;
			if (firstRun)
			{
				debugMesh.SetupMesh();
				firstRun = false;
			}
			debugMesh.Update();

			// TODO: Move this inside model class
			if (enableModelMesh)
			{
				animationShader.lock()->Bind();
				animationShader.lock()->SetUniformInt(0, animationStorage.GetDiffuseTextureFromCurrentlyBoundIndex()->GetTextureName());
				animationShader.lock()->SetUniformMatrix4F(projection, "projection");
				animationShader.lock()->SetUniformMatrix4F(view, "view");
				animationShader.lock()->SetUniformMatrix4F(model, "model");
				for (unsigned i = 0; i < animator->GetFinalBoneMatrices().size(); ++i)
				{
					std::string uniformName = "finalBonesMatrices[" + std::to_string(i) + "]";
					animationShader.lock()->SetUniformMatrix4F(animator->GetFinalBoneMatrices()[i], uniformName);
				}
				animationStorage.GetModelForCurrentlyBoundIndex()->Draw(animationShader.lock());
				animationShader.lock()->UnBind();
			}
			
			gridMesh.Update(gridShader.lock(), projection, view);

			GraphicsAPI::GetContext()->EnablePointSize(true);
			curveMesh->Draw();
			GraphicsAPI::GetContext()->EnablePointSize(false);

			modelManager->Update();

			window->Update();
		}

		assetManager->ClearStores();

		application->PostUpdate();
	}

	bool CoreEngine::Shutdown()
	{
		application->Shutdown();

		running = false;
		return true;
	}

	void CoreEngine::ProcessInput() const
	{
		const auto camera = Camera::GetInstance();

		const auto glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());

		if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		    glfwSetWindowShouldClose(glfwWindow, true);

		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_8) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::FORWARD);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_2) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::BACKWARD);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_4) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::LEFT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_6) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::RIGHT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_9) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::LOOK_UP);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_3) == GLFW_PRESS)
		    camera->ProcessKeyboard(CameraMovement::LOOK_DOWN);

		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_0) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_LEFT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_RIGHT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_IN);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_OUT);

		// IK_Target Movement Controls (Testing)
		//if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
		//    targetPoint->ProcessKeyboard(MovementType::FORWARD);
		//if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
		//    targetPoint->ProcessKeyboard(MovementType::BACKWARD);
		//if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
		//    targetPoint->ProcessKeyboard(MovementType::LEFT);
		//if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
		//    targetPoint->ProcessKeyboard(MovementType::RIGHT);
		//if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		//    targetPoint->ProcessKeyboard(MovementType::UP);
		//if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		//    targetPoint->ProcessKeyboard(MovementType::DOWN);

		//static bool isChangeModelKeyPressed = false;
		//if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		//{
		//	if (!isChangeModelKeyPressed)
		//	{
		//		//animationStorage.ChangeModel();
		//		//animator->ChangeAnimation(animationStorage.GetAnimationForCurrentlyBoundIndex());
		//		isChangeModelKeyPressed = true;
		//	}
		//}
		//else
		//{
		//	isChangeModelKeyPressed = false;
		//}

		// Disabled for testing
		//static bool isEnableModelKeyPressed = false;
		//if (glfwGetKey(glfwWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
		//{
		//	if (!isEnableModelKeyPressed)
		//	{
		//		enableModelMesh = !enableModelMesh;
		//		isEnableModelKeyPressed = true;
		//	}
		//}
		//else
		//{
		//	isEnableModelKeyPressed = false;
		//}

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

		static bool resetPathFollowing = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_R) == GLFW_PRESS)
		{
			if (!resetPathFollowing)
			{
				modelManager->Reset();
				resetPathFollowing = true;
			}
		}
		else
		{
			resetPathFollowing = false;
		}

		static unsigned path = 0;
		static bool changePath = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_1) == GLFW_PRESS)
		{
			if (changePath || path == 1)
				return;

			targetPoint->SetTargetLocation({ -50.0f, 15.0f, -50.0f });
			curveMesh->CreateNewSplinePath(targetPoint->GetControlPoints());
			modelManager->Reset();

			changePath = true;
			path = 1;
		}
		else if(glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS)
		{
			if (changePath || path == 2)
				return;

			targetPoint->SetTargetLocation({ 50.0f, 15.0f, -50.0f });
			curveMesh->CreateNewSplinePath(targetPoint->GetControlPoints());
			modelManager->Reset();

			changePath = true;
			path = 2;
		}
		else if(glfwGetKey(glfwWindow, GLFW_KEY_3) == GLFW_PRESS)
		{
			if (changePath || path == 3)
				return;

			targetPoint->SetTargetLocation({ 50.0f, 15.0f, 50.0f });
			curveMesh->CreateNewSplinePath(targetPoint->GetControlPoints());
			modelManager->Reset();

			changePath = true;
			path = 3;
		}
		else if(glfwGetKey(glfwWindow, GLFW_KEY_4) == GLFW_PRESS)
		{
			if (changePath || 4 == path)
				return;

			targetPoint->SetTargetLocation({ -50.0f, 15.0f, 50.0f });
			curveMesh->CreateNewSplinePath(targetPoint->GetControlPoints());
			modelManager->Reset();

			changePath = true;
			path = 4;
		}
		else
		{
			changePath = false;
		}
	}
}
