#include <AnimationPch.h>

#include "Animator.h"

#include "Animation/Animation.h"
#include "Core/Utilities/Time.h"
#include "DataTypes/AssimpNodeData.h"
#include "Core/Utilities/Utilites.h"

namespace AnimationEngine
{
	Animator::Animator()
		:	currentAnimation(nullptr),
			currentTime(0.0f),
			animationSpeedFactor(1.0f)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
		}
	}

	Animator::Animator(Animation* animation)
		:	currentAnimation(animation),
			currentTime(0.0f),
			animationSpeedFactor(1.0f)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
		}
	}

	void Animator::ChangeAnimation(Animation* newAnimation)
	{
		currentAnimation = newAnimation;
		currentTime = 0.0f;
	}

	void Animator::UpdateAnimation()
	{
		const auto dt = Time::GetDeltaTime();

		if (currentAnimation)
		{
			currentTime += currentAnimation->GetTicksPerSecond() * dt * animationSpeedFactor;
			currentTime = std::fmod(currentTime, currentAnimation->GetDuration());
			CalculateBoneTransformWithVQS(&currentAnimation->GetRootNode(), Math::VQS());
		}
	}

	void Animator::PlayAnimation(Animation* animation)
	{
		currentAnimation = animation;
		currentTime = 0.0f;
	}

	void Animator::ResetAnimation()
	{
		currentTime = 0.0f;
	}

	void Animator::CalculateBoneTransformWithVQS(const AssimpNodeData* node, Math::VQS parentVQS)
	{
		std::string nodeName = node->name;
		auto localVQS = Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(node->transformation);

		auto bone = currentAnimation->FindBone(nodeName);
		if (bone)
		{
			bone->Update(currentTime);
			localVQS = bone->GetLocalVQS();

			ExtractParentJointAndChildJoints(parentVQS, localVQS);
		}

		const auto worldVQS = parentVQS * localVQS;

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.contains(nodeName))
		{
			const int index = boneInfoMap[nodeName].id;
			const glm::mat4 offset = boneInfoMap[nodeName].offset;

			finalBoneMatrices[index] = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(worldVQS) * offset;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i)
			CalculateBoneTransformWithVQS(&node->children[i], worldVQS);
	}

	const std::vector<glm::mat4>& Animator::GetFinalBoneMatrices() const
	{
		return finalBoneMatrices;
	}

	const std::vector<Math::Vector3F>& Animator::GetJointPositions() const
	{
		return jointPositions;
	}

	float Animator::GetAnimationSpeedFactor() const
	{
		return animationSpeedFactor;
	}

	void Animator::SetAnimationSpeedFactor(float value)
	{
		animationSpeedFactor = value;
	}

	void Animator::ClearJoints()
	{
		jointPositions.clear();
	}

	void Animator::ExtractParentJointAndChildJoints(const Math::VQS& parent, const Math::VQS& child)
	{
		const auto parentJoint = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parent);
		const auto childJoint = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parent * child);

		const auto parentJointPosition = Math::Vector3F{ parentJoint[3].x, parentJoint[3].y, parentJoint[3].z };
		const auto childJointPosition = Math::Vector3F{ childJoint[3].x, childJoint[3].y, childJoint[3].z };

		jointPositions.push_back(parentJointPosition);
		jointPositions.push_back(childJointPosition);
	}
}
