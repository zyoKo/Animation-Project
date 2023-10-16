#include <AnimationPch.h>

#include <GLFW/glfw3.h>

#include "Application.h"

#include <execution>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Animation/Animator.h"
#include "Core/Logger/Log.h"
#include "Graphics/GraphicsAPI.h"
#include "AssetManager/AssetManager.h"
#include "Components/GridMesh.h"
#include "Components/Model.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/CameraConstants.h"

namespace Animator
{
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
		:	deltaTime(0.0f),
			lastFrame(0.0f),
			animator(std::make_shared<AnimatorR>()),
			assetManager(std::make_shared<AssetManager>())
	{
		if (!instance)
		{
			Log::Initialize();
		}

		ANIM_ASSERT(!instance, "Application Already exists!");
		instance = this;

		window = std::unique_ptr<IWindow>(IWindow::Create({ name, width, height }));

		// Bind Event Callback here
	}

	void Application::Initialize()
	{
		const std::string dreyarDiffuseTextureFile = "./assets/dreyar/textures/Dreyar_diffuse.png";
		const auto dreyarextureDiffuse = assetManager->CreateTexture(dreyarDiffuseTextureFile);
		dreyarextureDiffuse->SetTextureName("texture_diffuse1");

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
		const std::string dreyar1ColladaFile = "./assets/dreyar/Capoeira.dae";
		const std::string dreyar2ColladaFile = "./assets/dreyar/Dying.dae";
		const std::string dreyar3ColladaFile = "./assets/dreyar/JumpPushUp.dae";
		animationStorage.AddAssetToStorage(dreyar1ColladaFile, dreyarextureDiffuse);
		animationStorage.AddAssetToStorage(dreyar2ColladaFile, dreyarextureDiffuse);
		animationStorage.AddAssetToStorage(dreyar3ColladaFile, dreyarextureDiffuse);
	}

	void Application::Run()
	{
		auto shader = assetManager->RetrieveShaderFromStorage("AnimationShader");
		auto debugShader = assetManager->RetrieveShaderFromStorage("DebugAnimationShader");
		auto gridShader = assetManager->RetrieveShaderFromStorage("GridShader");

		auto textureDiffuse = assetManager->RetrieveTextureFromStorage("Dreyar_diffuse");
		auto gridTexture = assetManager->RetrieveTextureFromStorage("grid");

		animator->ChangeAnimation(animationStorage.GetAnimationForCurrentlyBoundIndex());

		GraphicsAPI::GetContext()->EnableDepthTest(true);
		//GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, GL_LINE);

		Camera camera(glm::vec3(0.0f, 8.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), CAMERA_YAW, CAMERA_PITCH);
		DebugMesh debugMesh(debugShader, &camera);
		GridMesh gridMesh;
		gridMesh.SetGridTexture(gridTexture);

		while (running && !window->WindowShouldClose())
		{
			GraphicsAPI::GetContext()->ClearColor();
			GraphicsAPI::GetContext()->ClearBuffer();

			const auto currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// camera/view transformation
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 10000.0f);
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			ProcessInput(camera);
			animator->UpdateAnimation(deltaTime);

			debugMesh.OverwriteJointsPosition(animator->GetJointPositions());
			animator->ClearJoints();
			static bool firstRun = true;
			if (firstRun)
			{
				debugMesh.SetupMesh();
				firstRun = false;
			}
			debugMesh.Update();

			shader->Bind();
			shader->SetUniformInt(0, animationStorage.GetDiffuseTextureFromCurrentlyBoundIndex()->GetTextureName());
			shader->SetUniformMatrix4F(projection, "projection");
			shader->SetUniformMatrix4F(view, "view");
			shader->SetUniformMatrix4F(model, "model");
			for (unsigned i = 0; i < animator->GetFinalBoneMatrices().size(); ++i)
			{
				std::string uniformName = "finalBonesMatrices[" + std::to_string(i) + "]";
				shader->SetUniformMatrix4F(animator->GetFinalBoneMatrices()[i], uniformName);
			}
			animationStorage.GetModelForCurrentlyBoundIndex()->Draw(shader);
			shader->UnBind();

			gridMesh.Update(gridShader, projection, view);

			window->Update();
		}

		assetManager->ClearStores();
	}

	void Application::Render()
	{
	}

	bool Application::Shutdown()
	{
		running = false;
		return true;
	}

	void Application::ProcessInput(Camera& camera)
	{
		const auto glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());

		if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		    glfwSetWindowShouldClose(glfwWindow, true);

		if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		    camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
		if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		    camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
		if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		    camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
		if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		    camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
		if (glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS)
			camera.ProcessKeyboard(CameraMovement::ROTATE_LEFT, deltaTime);
		if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS)
			camera.ProcessKeyboard(CameraMovement::ROTATE_RIGHT, deltaTime);

		static bool isChangeModelKeyPressed = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_T) == GLFW_PRESS)
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
	}
}
