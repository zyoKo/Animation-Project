#include <AnimationPch.h>

#include <GLFW/glfw3.h>

#include "Application.h"

#include <execution>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "Core/Logger/Log.h"
#include "Graphics/RenderApi.h"
#include "AssetManager/AssetManager.h"
#include "Components/Model.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/CameraConstants.h"
//#include "Core/Logger/GLDebug.h"
#include "Core/Utilities/Utilites.h"

namespace Animator
{
	static std::vector<Math::Vector3F> ExtractBonePositionsFromBoneTransformation(const std::vector<glm::mat4>& transform, int boneCount)
	{
		ANIM_ASSERT(transform.size() > boneCount, "Transformation size cannot be less than bone count!");
		std::vector<Math::Vector3F> bonesPositions;

		for (int i = 0; i < boneCount; ++i)
		{
			Math::Vector3F parentPosition = { transform[i][3][0], transform[i][3][1], transform[i][3][2] };
			Math::Vector3F childPosition  = { transform[i + 1][3][0], transform[i + 1][3][1], transform[i + 1][3][2] };

			bonesPositions.push_back(parentPosition);
			bonesPositions.push_back(childPosition);
		}

		return bonesPositions;
	}
}

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

		const std::string vampireDiffuseTextureFile = "./assets/vamp/textures/Vampire_diffuse.png";
		const auto vampireTextureDiffuse = assetManager->CreateTexture(dreyarDiffuseTextureFile);
		vampireTextureDiffuse->SetTextureName("texture_diffuse1");

		const std::string vertexShaderFile = "./assets/shaders/anim_model.vert";
		const std::string fragmentShaderFile = "./assets/shaders/anim_model.frag";
		assetManager->CreateShader("AnimationShader", vertexShaderFile, fragmentShaderFile);

		const std::string debugVertexShaderFile = "./assets/shaders/debug_anim_model.vert";
		const std::string debugFragShaderFile = "./assets/shaders/debug_anim_model.frag";
		assetManager->CreateShader("DebugAnimationShader", debugVertexShaderFile, debugFragShaderFile);

		// Adding Model And Animation to Storage
		const std::string dreyarColladaFile = "./assets/dreyar/Capoeira.dae";
		animationStorage.AddAssetToStorage(dreyarColladaFile, dreyarextureDiffuse);
		const std::string botColladaFile = "./assets/vamp/dancing_vampire.dae";
		animationStorage.AddAssetToStorage(botColladaFile, vampireTextureDiffuse);
	}

	void Application::Run()
	{
		auto shader = assetManager->RetrieveShaderFromStorage("AnimationShader");
		auto debugShader = assetManager->RetrieveShaderFromStorage("DebugAnimationShader");
		auto textureDiffuse = assetManager->RetrieveTextureFromStorage("Dreyar_diffuse");

		//const std::string dreyarColladaFile = "./assets/dreyar/Capoeira.dae";
		//const std::string bolColladaFile = "./assets/models/Walking.dae";
		//Model dreyarModel(dreyarColladaFile);
		//Model botModelFile(bolColladaFile);
		//dreyarModel.SetDiffuseTextureForMeshes(textureDiffuse);
		//Animation dreyarAnimation(dreyarColladaFile, &dreyarModel);
		//Animation botAnimationFile(bolColladaFile, &dreyarModel);

		//AnimatorR animator(&dreyarAnimation);
		animator->ChangeAnimation(animationStorage.GetAnimationForCurrentlyBoundIndex());

		RenderApi::GetContext()->EnableDepthTest(true);
		//GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, GL_LINE);

		Camera camera(glm::vec3(0.0f, 8.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), CAMERA_YAW, CAMERA_PITCH);
		DebugMesh debugMesh(debugShader, &camera);

		while (running && !window->WindowShouldClose())
		{
			RenderApi::GetContext()->ClearColor();
			RenderApi::GetContext()->ClearBuffer();

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
			shader->SetUniformInt(0, textureDiffuse->GetTextureName());
			shader->SetUniformMatrix4F(projection, "projection");
			shader->SetUniformMatrix4F(view, "view");
			shader->SetUniformMatrix4F(model, "model");
			for (unsigned i = 0; i < animator->GetFinalBoneMatrices().size(); ++i)
			{
				std::string uniformName = "finalBonesMatrices[" + std::to_string(i) + "]";
				shader->SetUniformMatrix4F(animator->GetFinalBoneMatrices()[i], uniformName);
			}
			//dreyarModel->Draw(shader);
			animationStorage.GetModelForCurrentlyBoundIndex()->Draw(shader);
			shader->UnBind();

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

		static bool isKeyPressed = false;
		if (glfwGetKey(glfwWindow, GLFW_KEY_T) == GLFW_PRESS)
		{
			if (!isKeyPressed)
			{
				animationStorage.ChangeModel();
				animator->ChangeAnimation(animationStorage.GetAnimationForCurrentlyBoundIndex());
				isKeyPressed = true;
			}
		}
		else
		{
			isKeyPressed = false;
		}
	}

	void Application::ChangeModelAndAnimation()
	{

	}
}
