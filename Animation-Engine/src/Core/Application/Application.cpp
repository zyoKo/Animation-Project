#include <AnimationPch.h>

#include <GLFW/glfw3.h>

#include "Application.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Core/Logger/Log.h"
#include "Graphics/RenderApi.h"
#include "AssetManager/AssetManager.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/CameraConstants.h"

namespace Animator
{
	Application* Application::instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
		:	deltaTime(0.0f),
			lastFrame(0.0f),
			assetManager(std::make_shared<AssetManager>()),
			camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), CAMERA_YAW, CAMERA_PITCH)
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
		const std::string textureFile = "./assets/digipen.jpg";
		texture = assetManager->CreateTexture(textureFile);
		texture->Bind(0);

		mesh = std::make_shared<Mesh>();

		const std::string vertexShaderFile = "./assets/basic.vert";
		const std::string fragmentShaderFile = "./assets/basic.frag";

		shader = assetManager->CreateShader("SimpleShader", vertexShaderFile, fragmentShaderFile);
		shader->Bind();
		shader->SetUniformInt(0, "ourTexture");

		RenderApi::GetContext()->EnableDepthTest(true);

		glm::mat4 transform = glm::mat4(1.0f);
		
		while (running && !window->WindowShouldClose())
		{
			const auto currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			ProcessInput();

			RenderApi::GetContext()->ClearColor();
			RenderApi::GetContext()->ClearBuffer();
			
			transform = glm::rotate(transform, deltaTime * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			shader->SetUniformMatrix4F(transform, "model");

			// pass projection matrix to shader (note that in this case it could change every frame)
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
			shader->SetUniformMatrix4F(projection, "projection");

			// camera/view transformation
			glm::mat4 view = camera.GetViewMatrix();
			shader->SetUniformMatrix4F(view, "view");

			mesh->Draw(shader);

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
