#pragma once

#include "CameraProperties.h"

namespace Animator
{
	class Camera
	{
	public:
		Camera(float width, float height, float fieldOfView, float nearPlane, float farPlane);

		explicit Camera(const Camera&) = delete;

		const Camera& operator=(const Camera&) = delete;

		~Camera() = default;

		const glm::vec3& GetCameraPosition() const;

		glm::vec3& GetCameraPosition();

		void SetCameraPosition(const glm::vec3& position);

		const glm::mat4& GetProjectionMatrix() const;

		glm::mat4& GetProjectionMatrix();

		const glm::mat4& GetViewMatrix() const;

		glm::mat4& GetViewMatrix();

		const glm::mat4& GetViewProjectionMatrix() const;

		glm::mat4& GetViewProjectionMatrix();

		void SetViewAndProjectionMatrix(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		void CalculateViewProjectionMatrix();

	private:
		CameraProperties cameraProperties;

		glm::vec3 cameraOrigin;

		glm::vec3 cameraLookAt;
	};
}
