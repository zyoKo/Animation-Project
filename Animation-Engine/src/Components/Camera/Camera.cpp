#include <AnimationPch.h>

#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "CameraConstants.h"
#include "Core/Logger/Log.h"

namespace Animator
{
	Camera::Camera(glm::vec3 position = CAMERA_DEFAULT_POSITION, glm::vec3 worldUp = CAMERA_DEFAULT_WORLD_UP, float yaw = CAMERA_YAW, float pitch = CAMERA_PITCH)
		:	cameraPosition(position),
			worldUp(worldUp),
			yaw(yaw),
			pitch(pitch),
			movementSpeed(CAMERA_SPEED),
			mouseSensitivity(CAMERA_SENSITIVITY),
			zoom(CAMERA_ZOOM)
	{
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

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(cameraPosition, cameraPosition + front, up);
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

	//Camera::Camera(float width, float height, float fieldOfView, float nearPlane, float farPlane)
	//	:	cameraProperties{fieldOfView, width, height, nearPlane, farPlane }
	//{
	//	//cameraProperties.nearPlane = 0.1f;
	//	//cameraProperties.farPlane = 1000.0f;
	//	//cameraProperties.fieldOfView = 45.0f;
	//	//cameraProperties.width = 800.0f;
	//	//cameraProperties.height = 600.0f;

	//	cameraProperties.nearPlane = nearPlane;
	//	cameraProperties.farPlane = farPlane;
	//	cameraProperties.fieldOfView = glm::radians(fieldOfView);
	//	cameraProperties.width = width;
	//	cameraProperties.height = height;

	//	CalculateViewProjectionMatrix();
	//}

	/*const glm::mat4& Camera::GetProjectionMatrix() const
	{
		ANIM_ASSERT(!cameraProperties.IsComplete(), "Set all values camera properties before retriving projection matrix!");
		return cameraProperties.projectionMatrix.value();
	}

	glm::mat4& Camera::GetProjectionMatrix()
	{
		ANIM_ASSERT(!cameraProperties.IsComplete(), "Set all values camera properties before retriving projection matrix!");
		return cameraProperties.projectionMatrix.value();
	}

	const glm::mat4& Camera::GetViewMatrix() const
	{
		ANIM_ASSERT(!cameraProperties.IsComplete(), "Set all values camera properties before retriving projection matrix!");
		return cameraProperties.viewMatrix.value();
	}

	glm::mat4& Camera::GetViewMatrix()
	{
		ANIM_ASSERT(!cameraProperties.IsComplete(), "Set all values camera properties before retriving projection matrix!");
		return cameraProperties.viewMatrix.value();
	}

	const glm::mat4& Camera::GetViewProjectionMatrix() const
	{
		ANIM_ASSERT(!cameraProperties.IsComplete(), "Set all values camera properties before retriving projection matrix!");
		return cameraProperties.viewProjectionMatrix.value();
	}

	glm::mat4& Camera::GetViewProjectionMatrix()
	{
		ANIM_ASSERT(!cameraProperties.IsComplete(), "Set all values camera properties before retriving projection matrix!");
		return cameraProperties.viewProjectionMatrix.value();
	}

	void Camera::SetViewAndProjectionMatrix(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		this->cameraProperties.viewMatrix = viewMatrix;
		this->cameraProperties.projectionMatrix = projectionMatrix;
	}

	void Camera::CalculateViewProjectionMatrix()
	{
		ANIM_ASSERT(!cameraProperties.CanComputeViewProjection(), "Need to setup other values before computing ViewProjection Matrix!");

		cameraProperties.projectionMatrix = glm::perspective(
			cameraProperties.fieldOfView.value(),
			cameraProperties.width.value() / cameraProperties.height.value(),
			cameraProperties.nearPlane.value(), 
			cameraProperties.farPlane.value());

		cameraProperties.viewMatrix = glm::translate(
			glm::mat4(1.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f));
	}*/
}
