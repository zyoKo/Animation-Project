#include <AnimationPch.h>

#include "Sphere.h"

#include "Animation/Model.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/Constants/CameraConstants.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/Utilities/Utilites.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine::Physics
{
	Sphere::Sphere(const Math::Vec3F& center, float radius)
		:	center(center),
			radius(radius)
	{ }

	void Sphere::Initialize(std::string_view filePath)
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();
		const std::string& vertexShaderPath = "./assets/shaders/sphere.vert";
		const std::string& fragmentShaderPath = "./assets/shaders/sphere.frag";
		shader = assetManager->CreateShader("SphereShader", vertexShaderPath, fragmentShaderPath);

		sphereModel = std::make_shared<Model>(std::string(filePath));
	}

	void Sphere::Render() const
	{
		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			return;
		}

		const auto* camera = Camera::GetInstance();
		const auto viewMatrix = camera->GetViewMatrix();
		const auto projectionMatrix = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);

		auto model = glm::mat4(1.0f);
		const auto translate = glm::translate(model, Utils::GLMInternalHelper::ConvertInternalVectorToGLM(center));
		const auto scale = glm::scale(model, glm::vec3(radius - 0.5f));

		model = model * translate * scale;

		shaderPtr->Bind();
		shaderPtr->SetUniformMatrix4F(viewMatrix, "view");
		shaderPtr->SetUniformMatrix4F(projectionMatrix, "projection");
		shaderPtr->SetUniformMatrix4F(model, "model");

		sphereModel->Draw(shaderPtr);

		shaderPtr->UnBind();
	}

	const Math::Vec3F& Sphere::GetCenter() const
	{
		return center;
	}

	Math::Vec3F& Sphere::GetCenter()
	{
		return center;
	}

	float& Sphere::GetRadius()
	{
		return radius;
	}
}
