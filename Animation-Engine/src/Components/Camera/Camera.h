#pragma once

#include "Type/CameraMovement.h"

namespace AnimationEngine
{
	class Camera
	{
	public:
		static Camera* GetInstance();

		void Initialize();

		void SetCameraPosition(const glm::vec3& position);
		const glm::vec3& GetCameraPosition() const;

		void SetYaw(float yaw);
		float GetYaw() const;

		void SetPitch(float pitch);
		float GetPitch() const;

		void SetMovementSpeed(float speed);
		float GetMovementSpeed() const;

		void SetMouseSensitivity(float sensitivity);
		float GetMouseSensitivity() const;

		void SetZoom(float zoom);
		float GetZoom() const;

		void SetZoomSpeed(float speed);
		float GetZoomSpeed() const;

		glm::mat4 GetViewMatrix() const;

		void ProcessKeyboard(CameraMovement direction);

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
