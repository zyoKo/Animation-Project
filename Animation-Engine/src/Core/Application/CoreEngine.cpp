#include <AnimationPch.h>

#include "CoreEngine.h"

#include <GLFW/glfw3.h>
#include <execution>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Animation/Animator.h"
#include "Core/Logger/Log.h"
#include "Graphics/GraphicsAPI.h"
#include "AssetManager/AssetManager.h"
#include "Components/GridMesh.h"
#include "Animation/Model.h"
#include "Components/CurveMesh.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/Constants/CameraConstants.h"
#include "Core/ServiceLocators/AssetManagerLocator.h"
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Utilites.h"
#include "Interface/IApplication.h"

namespace AnimationEngine
{
	CoreEngine::CoreEngine(const std::string& name, uint32_t width, uint32_t height)
		:	animator(std::make_shared<Animator>()),
			assetManager(new AssetManager()),
			point(nullptr)
	{
		Log::Initialize();

		window = std::unique_ptr<IWindow>(IWindow::Create({ name, width, height }));

		// Bind Event Callback here

		AssetManagerLocator::Initialize();

		AssetManagerLocator::Provide(assetManager);
	}

	CoreEngine::~CoreEngine()
	{
		delete assetManager;
		assetManager = nullptr;
	}

	void CoreEngine::SetApplication(const std::shared_ptr<IApplication>& app)
	{
		this->application = app;
	}

	void CoreEngine::Initialize()
	{
		Camera::GetInstance()->Initialize();

		application->Initialize();

		const auto assetManager = AssetManagerLocator::GetAssetManager();

		const std::string dreyarDiffuseTextureFile = "./assets/dreyar/textures/Dreyar_diffuse.png";
		const auto dreyarTextureDiffuse = assetManager->CreateTexture(dreyarDiffuseTextureFile);
		dreyarTextureDiffuse->SetTextureName("texture_diffuse1");

		const std::string gridTextureFile = "./assets/grid.png";
		const auto gridTexture = assetManager->CreateTexture(gridTextureFile);
		gridTexture->SetTextureName("gridTexture");

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
		animationStorage.AddAssetToStorage(dreyar2ColladaFile, dreyarTextureDiffuse);
		animationStorage.AddAssetToStorage(dreyar3ColladaFile, dreyarTextureDiffuse);

		// Spline
		const std::string splineVertShader = "./assets/shaders/spline.vert";
		const std::string splineFragShader = "./assets/shaders/spline.frag";
		assetManager->CreateShader("SplineShader", splineVertShader, splineFragShader);
	}

	void CoreEngine::Update()
	{
		application->PreUpdate();

		const auto assetManager = AssetManagerLocator::GetAssetManager();

		auto animationShader = assetManager->RetrieveShaderFromStorage("AnimationShader");
		auto debugShader = assetManager->RetrieveShaderFromStorage("DebugAnimationShader");
		auto gridShader = assetManager->RetrieveShaderFromStorage("GridShader");
		auto splineShader = assetManager->RetrieveShaderFromStorage("SplineShader");

		auto textureDiffuse = assetManager->RetrieveTextureFromStorage("Dreyar_diffuse");
		auto gridTexture = assetManager->RetrieveTextureFromStorage("grid");

		animator->ChangeAnimation(animationStorage.GetAnimationForCurrentlyBoundIndex());

		GraphicsAPI::GetContext()->EnableDepthTest(true);
		GraphicsAPI::GetContext()->EnableWireFrameMode(false);

		auto camera = Camera::GetInstance();

		DebugMesh debugMesh(debugShader);
		GridMesh gridMesh;
		gridMesh.SetGridTexture(gridTexture);

		CurveMesh curveMesh;
		curveMesh.SetShader(splineShader);

		point = new PointMesh();
		point->SetSpline(curveMesh.GetSpline());

		while (running && !window->WindowShouldClose())
		{
			GraphicsAPI::GetContext()->ClearColor();
			GraphicsAPI::GetContext()->ClearBuffer();

			Time::Update();

			application->Update();

			// camera/view transformation
			glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)window->GetWidth() / (float)window->GetHeight(), CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
			glm::mat4 view = camera->GetViewMatrix();
			glm::mat4 model = glm::mat4(1.0f);

			ProcessInput();
			animator->UpdateAnimation();

			debugMesh.OverwriteJointsPosition(animator->GetJointPositions());
			animator->ClearJoints();
			static bool firstRun = true;
			if (firstRun)
			{
				debugMesh.SetupMesh();
				firstRun = false;
			}
			debugMesh.Update();

			if (enableModelMesh)
			{
				animationShader->Bind();
				animationShader->SetUniformInt(0, animationStorage.GetDiffuseTextureFromCurrentlyBoundIndex()->GetTextureName());
				animationShader->SetUniformMatrix4F(projection, "projection");
				animationShader->SetUniformMatrix4F(view, "view");
				animationShader->SetUniformMatrix4F(model, "model");
				for (unsigned i = 0; i < animator->GetFinalBoneMatrices().size(); ++i)
				{
					std::string uniformName = "finalBonesMatrices[" + std::to_string(i) + "]";
					animationShader->SetUniformMatrix4F(animator->GetFinalBoneMatrices()[i], uniformName);
				}
				animationStorage.GetModelForCurrentlyBoundIndex()->Draw(animationShader);
				animationShader->UnBind();
			}
			
			gridMesh.Update(gridShader, projection, view);
			
			curveMesh.Draw();

			GraphicsAPI::GetContext()->EnablePointSize(true);
			point->Draw();
			GraphicsAPI::GetContext()->EnablePointSize(false);

			auto newRotation = Utils::GLMInternalHelper::ConvertQuaternionToGLMMatrix(point->GetRotation());

			auto& animationRoot = animationStorage.GetAnimationForCurrentlyBoundIndex()->GetRootNode();

			glm::vec3 scale, translation, skew;
			glm::vec4 perspective;
			glm::quat orientation;
			glm::decompose(animationRoot.transformation, scale, orientation, translation, skew, perspective);

			auto newMatrix = glm::mat4(1.0f);
			newMatrix = glm::scale(newMatrix, scale); // Apply scale
			newMatrix = newMatrix * newRotation;
			newMatrix = glm::translate(newMatrix, translation); // Apply translation

			animationRoot.transformation = newMatrix;

			animationRoot.transformation[3][0] = point->GetPosition().x;
			animationRoot.transformation[3][1] = point->GetPosition().y;
			animationRoot.transformation[3][2] = point->GetPosition().z;

			//LOG_WARN("Incoming Rotation: ({0}, {1}, {2}, {3})", point->GetRotation().x, point->GetRotation().y, point->GetRotation().z, point->GetRotation().w);
			auto axis = Math::QuatF::GetAxis(point->GetRotation());
			LOG_WARN("Angle: {0}_________Axis: ({1}, {2}, {3})", Math::QuatF::GetAngle(point->GetRotation()) * (180.0f / 3.14f), axis.x, axis.y, axis.z);
			//auto rotationAxis = Utils::GLMInternalHelper::ConvertInternalVectorToGLM(Math::QuatF::GetAxis(point->GetRotation()));

			window->Update();
		}

		assetManager->ClearStores();

		delete point;

		application->PostUpdate();
	}

	bool CoreEngine::Shutdown()
	{
		running = false;
		return true;
	}

	void CoreEngine::ProcessInput()
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

		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_LEFT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_0) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ROTATE_RIGHT);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_IN);
		if (glfwGetKey(glfwWindow, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
			camera->ProcessKeyboard(CameraMovement::ZOOM_OUT);

		static bool isChangeModelKeyPressed = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!isChangeModelKeyPressed)
			{
				animationStorage.ChangeModel();
				animator->ChangeAnimation(animationStorage.GetAnimationForCurrentlyBoundIndex());
				isChangeModelKeyPressed = true;
			}
		}
		else
		{
			isChangeModelKeyPressed = false;
		}

		static bool isEnableModelKeyPressed = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			if (!isEnableModelKeyPressed)
			{
				enableModelMesh = !enableModelMesh;
				isEnableModelKeyPressed = true;
			}
		}
		else
		{
			isEnableModelKeyPressed = false;
		}

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

		static constexpr float moveSpeed = 0.1f;
		static Math::Vector3F newPosition;
		if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			newPosition = { point->GetPosition().x, point->GetPosition().y, point->GetPosition().z - moveSpeed };
			point->SetPosition(newPosition);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			newPosition = { point->GetPosition().x - moveSpeed, point->GetPosition().y, point->GetPosition().z };
			point->SetPosition(newPosition);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			newPosition = { point->GetPosition().x + moveSpeed, point->GetPosition().y, point->GetPosition().z };
			point->SetPosition(newPosition);
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			newPosition = { point->GetPosition().x, point->GetPosition().y, point->GetPosition().z + moveSpeed };
			point->SetPosition(newPosition);
		}

		static bool resetPathFollowing = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_R) == GLFW_PRESS)
		{
			if (!resetPathFollowing)
			{
				point->Reset();
				resetPathFollowing = true;
			}
		}
		else
		{
			resetPathFollowing = false;
		}
	}
}
