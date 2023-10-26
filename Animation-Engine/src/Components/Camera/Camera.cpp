#include <AnimationPch.h>

#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

#include "CameraConstants.h"

namespace Animator
{
	Camera::Camera(glm::vec3 position = CAMERA_DEFAULT_POSITION, glm::vec3 worldUp = CAMERA_DEFAULT_WORLD_UP, float yaw = CAMERA_YAW, float pitch = CAMERA_PITCH)
		:	cameraPosition(position),
			worldUp(worldUp),
			rotateSpeed(CAMERA_ROTATE_SPEED),
			zoomSpeed(CAMERA_ZOOM_SPEED),
			yaw(yaw),
			pitch(pitch),
			movementSpeed(CAMERA_SPEED),
			mouseSensitivity(CAMERA_SENSITIVITY),
			zoom(CAMERA_ZOOM)
	{
		initialPosition = position;
		initialWorldUp = worldUp;
		initialYaw = yaw;
		initialPitch = pitch;
		initialZoom = zoom;

		UpdateCameraVectors();
	}

	const glm::vec3& Camera::GetCameraPosition() const
	{
		return cameraPosition;
	}

	glm::vec3& Camera::GetCameraPosition()
	{
		return cameraPosition;
	}

	void Camera::SetCameraPosition(const glm::vec3& position)
	{
		this->cameraPosition = position;
	}

	float Camera::GetZoom() const
	{
		return zoom;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(cameraPosition, cameraPosition + front, up);
	}

	void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
	{
		const float velocity = movementSpeed * deltaTime;

		if (direction == CameraMovement::FORWARD)
		    cameraPosition += front * velocity;
		if (direction == CameraMovement::BACKWARD)
		    cameraPosition -= front * velocity;
		if (direction == CameraMovement::LEFT)
		    cameraPosition -= right * velocity;
		if (direction == CameraMovement::RIGHT)
		    cameraPosition += right * velocity;
		if (direction == CameraMovement::ROTATE_LEFT)
			yaw -= rotateSpeed * deltaTime;
		if (direction == CameraMovement::ROTATE_RIGHT)
			yaw += rotateSpeed * deltaTime;
		if (direction == CameraMovement::ZOOM_IN)
		    zoom -= zoomSpeed * deltaTime;
		if (direction == CameraMovement::ZOOM_OUT)
		    zoom += zoomSpeed * deltaTime;

		if (zoom < 1.0f)
		    zoom = 1.0f;
		if (zoom > 45.0f)
		    zoom = 45.0f;

		UpdateCameraVectors();
	}

	void Camera::Reset()
	{
	    cameraPosition = initialPosition;
	    worldUp = initialWorldUp;
	    yaw = initialYaw;
	    pitch = initialPitch;
	    zoom = initialZoom;
	
	    UpdateCameraVectors();
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 tempFront;

		tempFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
		tempFront.y = std::sin(glm::radians(pitch));
		tempFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

		this->front = glm::normalize(tempFront);

		right = glm::normalize(glm::cross(this->front, worldUp));
		up = glm::normalize(glm::cross(right, this->front));
	}
}
