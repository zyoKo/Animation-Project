#pragma once

#include "Type/CameraMovement.h"

namespace AnimationEngine
{
	class Camera
	{
	public:
		Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch);

		static Camera* GetInstance() { return &instance; }

		const glm::vec3& GetCameraPosition() const;

		glm::vec3& GetCameraPosition();

		void SetCameraPosition(const glm::vec3& position);

		float GetZoom() const;

		glm::mat4 GetViewMatrix() const;

		void ProcessKeyboard(CameraMovement direction, float deltaTime);

		void Reset();

	private:
		static Camera instance;

		glm::vec3 cameraPosition;

		glm::vec3 front;

		glm::vec3 up;

		glm::vec3 right;

		glm::vec3 worldUp;

		float rotateSpeed;

		float zoomSpeed;

		float yaw, pitch;

		float movementSpeed;

		float mouseSensitivity;

		float zoom;

		// Reset Camera
		glm::vec3 initialPosition;

		glm::vec3 initialWorldUp;

		float initialYaw, initialPitch;

		float initialZoom;

		Camera() = default;

		void UpdateCameraVectors();
	};
}
