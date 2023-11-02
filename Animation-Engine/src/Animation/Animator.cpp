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
			currentTime(0.0f)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
		}
	}

	Animator::Animator(Animation* animation)
		:	currentAnimation(animation),
			currentTime(0.0f)
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
			currentTime += currentAnimation->GetTicksPerSecond() * dt;
			currentTime = std::fmod(currentTime, currentAnimation->GetDuration());
			CalculateBoneTransformWithVQS(&currentAnimation->GetRootNode(), Math::VQS());
		}
	}

	void Animator::PlayAnimation(Animation* animation)
	{
		currentAnimation = animation;
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

			jointPositions.push_back(ExtractJointPosition(Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parentVQS)));
			jointPositions.push_back(ExtractJointPosition(Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parentVQS * localVQS)));
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

	void Animator::ClearJoints()
	{
		jointPositions.clear();
	}

	Math::Vector3F Animator::ExtractJointPosition(const glm::mat4& transform)
	{
		return { transform[3][0], transform[3][1], transform[3][2] };
	}
}
