#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "CameraProperties.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Animator
{
	enum class CameraMovement
	{
		FORWARD,

		BACKWARD,

		LEFT,

		RIGHT,

		ROTATE_LEFT,

		ROTATE_RIGHT,

		ZOOM_IN,

		ZOOM_OUT
	};

	class Camera
	{
	public:
		Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch);

		~Camera() = default;

		const glm::vec3& GetCameraPosition() const;

		glm::vec3& GetCameraPosition();

		void SetCameraPosition(const glm::vec3& position);

		float GetZoom() const;

		glm::mat4 GetViewMatrix() const;

		void ProcessKeyboard(CameraMovement direction, float deltaTime);

		void Reset();

	private:
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

		void UpdateCameraVectors();
	};
}
