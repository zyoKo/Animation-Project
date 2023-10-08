#include <AnimationPch.h>

#include "ModelPose.h"

namespace Animator
{
	ModelPose::ModelPose()
		:	localTransform{ 1.0f }
	{
	}

	void ModelPose::Update(float animationTime)
	{

	}

	const std::vector<glm::vec3>& ModelPose::GetJointsPosition() const
	{
		return jointsPosition;
	}

	void ModelPose::SetJointsPosition(const std::vector<glm::vec3>& position)
	{
		this->jointsPosition = jointsPosition;
	}

	const std::vector<glm::quat>& ModelPose::GetJointsRotation() const
	{
		return jointsRotation;
	}

	void ModelPose::SetJointsRotation(const std::vector<glm::quat>& rotation)
	{
		this->jointsRotation = rotation;
	}

	const std::vector<glm::vec3>& ModelPose::GetJointsScale() const
	{
		return jointsScale;
	}

	void ModelPose::SetJointsScale(const std::vector<glm::vec3>& scale)
	{
		this->jointsScale = scale;
	}

	const glm::mat4& ModelPose::GetLocalTransform() const
	{
		return localTransform;
	}
}
