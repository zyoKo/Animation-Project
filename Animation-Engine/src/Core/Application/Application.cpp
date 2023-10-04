#include <AnimationPch.h>

#include <GLFW/glfw3.h>

#include "Application.h"

#include <assimp/postprocess.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "Core/Logger/Log.h"
#include "Graphics/RenderApi.h"
#include "AssetManager/AssetManager.h"
#include "Components/Model.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/CameraConstants.h"
#include "Core/Utilities/Utilites.h"

namespace Animator
{
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
		:	deltaTime(0.0f),
			lastFrame(0.0f),
			assetManager(std::make_shared<AssetManager>()),
			camera(glm::vec3(0.0f, 0.0f, 200.0f), glm::vec3(0.0f, 1.0f, 0.0f), CAMERA_YAW, CAMERA_PITCH)
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
	}

	void Application::Run()
	{
		const std::string textureFile = "./assets/model/textures/Dreyar_diffuse.png";
		textureDiffuse = assetManager->CreateTexture(textureFile);
		textureDiffuse->SetTextureName("texture_diffuse1");

		const std::string modelFile = "./assets/model/Capoeira.dae";
		Model bagModel(modelFile);
		bagModel.SetDiffuseTextureForMeshes(textureDiffuse);

		//const std::string animationFile = "./assets/vamp/dancing_vampire.dae";
		const std::string animationFile = "./assets/model/Capoeira.dae";
		Animation danceAnimation(animationFile, &bagModel);

		AnimatorR animator(&danceAnimation);

		const std::string vertexShaderFile = "./assets/shaders/anim_model.vert";
		const std::string fragmentShaderFile = "./assets/shaders/anim_model.frag";

		shader = assetManager->CreateShader("SimpleShader", vertexShaderFile, fragmentShaderFile);

		RenderApi::GetContext()->EnableDepthTest(true);
		//GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, GL_LINE);
		
		while (running && !window->WindowShouldClose())
		{
			const auto currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			ProcessInput();

			RenderApi::GetContext()->ClearColor();
			RenderApi::GetContext()->ClearBuffer();
			
			//transform = glm::rotate(transform, deltaTime * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			//shader->SetUniformMatrix4F(transform, "model");

			shader->Bind();
			shader->SetUniformInt(0, "texture_diffuse1");

			// pass projection matrix to shader (note that in this case it could change every frame)
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 10000.0f);
			shader->SetUniformMatrix4F(projection, "projection");

			// camera/view transformation
			glm::mat4 view = camera.GetViewMatrix();
			shader->SetUniformMatrix4F(view, "view");

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			shader->SetUniformMatrix4F(model, "model");

			animator.UpdateAnimation(deltaTime);

			//auto transform = animator.GetFinalBoneMatrices();
			for (unsigned i = 0; i < animator.GetFinalBoneMatrices().size(); ++i)
			{
				std::string uniformName = "finalBonesMatrices[" + std::to_string(i) + "]";
				shader->SetUniformMatrix4F(animator.GetFinalBoneMatrices()[i], uniformName);
			}

			bagModel.Draw(shader);

			shader->UnBind();

			// Swap buffer and poll events
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

	void Application::ProcessInput()
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
	}
}
