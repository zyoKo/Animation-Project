#include <AnimationPch.h>

#include "PointMesh.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Camera/Camera.h"
#include "Core/Logger/GLDebug.h"
#include "Core/ServiceLocators/AssetManagerLocator.h"
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Utilites.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Types/DebugDrawMode.h"

namespace AnimationEngine
{
	PointMesh::PointMesh()
		:	position(0.0f, 0.0f, 0.0f),
			moveSpeed(13.5f),
			totalArcLength(0.0f),
			distanceTraveled(0.0f),
			timeInSeconds(0.0f)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();

		SetupShader();
	}

	void PointMesh::SetPosition(const Math::Vector3F& newPosition)
	{
		position = newPosition;
	}

	const Math::Vector3F& PointMesh::GetPosition() const
	{
		return position;
	}

	const Math::QuatF& PointMesh::GetRotation() const
	{
		return rotation;
	}

	void PointMesh::SetSpline(const std::weak_ptr<Math::HermiteSpline>& spline)
	{
		this->spline = spline;
	}

	void PointMesh::Draw()
	{
		shader->Bind();

		UpdatePositionOnSpline();

		const auto camera = Camera::GetInstance();
		const glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		const glm::mat4 view = camera->GetViewMatrix();
		shader->SetUniformMatrix4F(projection, "projection");
		shader->SetUniformMatrix4F(view, "view");
		shader->SetUniformVector3F(position, "pointPosition");

		vertexArrayObject->Bind();
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::Points), 0, 1);
		vertexArrayObject->UnBind();

		shader->UnBind();
	}

	void PointMesh::Reset()
	{
		timeInSeconds = 0.0f;
		distanceTraveled = 0.0f;
	}

	void PointMesh::SetupShader()
	{
		const auto assetManager = AssetManagerLocator::GetAssetManager();

		const std::string vertexShaderFile = "./assets/shaders/point.vert";
		const std::string fragmentShaderFile = "./assets/shaders/point.frag";
		assetManager->CreateShader("PointShader", vertexShaderFile, fragmentShaderFile);

		shader = assetManager->RetrieveShaderFromStorage("PointShader");
	}

	void PointMesh::UpdatePositionOnSpline()
	{
		const auto splinePtr = spline.lock();
		if (!splinePtr)
		{
			return;
		}

		static bool firstRun = true;
		if (firstRun)
		{
			totalArcLength = splinePtr->GetCumulativeArcLength(1.0f);
			position = splinePtr->GetControlPoints()[0];
			firstRun = false;
		}

		timeInSeconds += Time::GetDeltaTime();

		const auto moveDistance = moveSpeed * timeInSeconds;
		//LOG_WARN("Distance To Move: {0}", moveDistance);

		distanceTraveled = moveDistance;
		static float delta = 0.1f;
		if (distanceTraveled < totalArcLength)
		{
			const auto nextPoint = splinePtr->FindPointOnCurve(distanceTraveled);

			if (distanceTraveled + delta < totalArcLength)
			{
				const auto centerOfInterest = splinePtr->FindPointOnCurve(distanceTraveled + delta);

				const auto direction = (centerOfInterest - nextPoint).GetNormalize();

				// To calculate the rotation quaternion, we need the axis and angle
				auto rotationAxis = Math::Vector3F::Cross(Math::Vector3F::GetForward(), direction);
				rotationAxis = rotationAxis.GetNormalize();
				const float rotationAngle = std::acos(Math::Vector3F::Dot(Math::Vector3F::GetForward(), direction));
				
				// Create a quaternion representing the rotation from defaultForward to direction
				rotation = Math::QuatF::AngleAxis(rotationAngle, rotationAxis);
			}

			position = nextPoint;
		}
	}
}
