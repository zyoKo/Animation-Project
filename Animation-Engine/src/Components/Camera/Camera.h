#pragma once

#include "CameraProperties.h"
#include "glad/glad.h"

namespace Animator
{
	enum class CameraMovement
	{
		FORWARD,

		BACKWARD,

		LEFT,

		RIGHT
	};

	class Camera
	{
	public:
		//Camera(float width, float height, float fieldOfView, float nearPlane, float farPlane);

		Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch);

		//explicit Camera(const Camera&) = delete;

		//const Camera& operator=(const Camera&) = delete;

		~Camera() = default;

		const glm::vec3& GetCameraPosition() const;

		glm::vec3& GetCameraPosition();

		void SetCameraPosition(const glm::vec3& position);

		glm::mat4 GetViewMatrix() const;

		void ProcessKeyboard(CameraMovement direction, float deltaTime)
		{
			float velocity = movementSpeed * deltaTime;
			if (direction == CameraMovement::FORWARD)
			    cameraPosition += front * velocity;
			if (direction == CameraMovement::BACKWARD)
			    cameraPosition -= front * velocity;
			if (direction == CameraMovement::LEFT)
			    cameraPosition -= front * velocity;
			if (direction == CameraMovement::RIGHT)
			    cameraPosition += front * velocity;
		}

		// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
		{
		    xoffset *= mouseSensitivity;
		    yoffset *= mouseSensitivity;

		    yaw   += xoffset;
		    pitch += yoffset;

		    // make sure that when pitch is out of bounds, screen doesn't get flipped
		    if (constrainPitch)
		    {
		        if (pitch > 89.0f)
		            pitch = 89.0f;
		        if (pitch < -89.0f)
		            pitch = -89.0f;
		    }

		    // update Front, Right and Up Vectors using the updated Euler angles
		    UpdateCameraVectors();
		}

		glm::vec3 cameraPosition;

		glm::vec3 front;

		glm::vec3 up;

		glm::vec3 right;

		glm::vec3 worldUp;

		float yaw, pitch;

		float movementSpeed;

		float mouseSensitivity;

		float zoom;

		/*const glm::mat4& GetProjectionMatrix() const;
		glm::mat4& GetProjectionMatrix();
		const glm::mat4& GetViewMatrix() const;
		glm::mat4& GetViewMatrix();
		const glm::mat4& GetViewProjectionMatrix() const;
		glm::mat4& GetViewProjectionMatrix();
		void SetViewAndProjectionMatrix(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
		void CalculateViewProjectionMatrix();*/

	private:
		void UpdateCameraVectors();

		/*CameraProperties cameraProperties;
		glm::vec3 cameraOrigin;
		glm::vec3 cameraLookAt;*/
	};
}
