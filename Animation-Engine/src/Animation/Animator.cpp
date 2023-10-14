#include <AnimationPch.h>

#include "Animator.h"

#include "Animation/Animation.h"
#include "DataTypes/AssimpNodeData.h"
#include "Math/VQM.h"

namespace Animator
{
	AnimatorR::AnimatorR()
		:	currentAnimation(nullptr),
			currentTime(0.0f),
			deltaTime(0.0f)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
		}
	}

	AnimatorR::AnimatorR(Animation* animation)
		:	currentAnimation(animation),
			currentTime(0.0f),
			deltaTime(0.0f)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
		}
	}

	void AnimatorR::ChangeAnimation(Animation* newAnimation)
	{
		currentAnimation = newAnimation;
		currentTime = 0.0f;
	}

	void AnimatorR::UpdateAnimation(float dt)
	{
		deltaTime = dt;

		if (currentAnimation)
		{
			currentTime += currentAnimation->GetTicksPerSecond() * dt;
			currentTime = std::fmod(currentTime, currentAnimation->GetDuration());
			CalculateBoneTransform(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void AnimatorR::PlayAnimation(Animation* animation)
	{
		currentAnimation = animation;
		currentTime = 0.0f;
	}

	void AnimatorR::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 localNodeTransform = node->transformation;

		Bone* bone = currentAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(currentTime);
			localNodeTransform = bone->GetLocalTransform();

			jointPositions.push_back(ExtractJointPosition(parentTransform));
			jointPositions.push_back(ExtractJointPosition(parentTransform * localNodeTransform));
		}

		const glm::mat4 worldTransform = parentTransform * localNodeTransform;

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.contains(nodeName))
		{
			const int index = boneInfoMap[nodeName].id;
			const glm::mat4 offset = boneInfoMap[nodeName].offset;

			finalBoneMatrices[index] = worldTransform * offset;
		}

		for (int i = 0; i < node->childrenCount; ++i)
			CalculateBoneTransform(&node->children[i], worldTransform);
	}

	const std::vector<glm::mat4>& AnimatorR::GetFinalBoneMatrices() const
	{
		return finalBoneMatrices;
	}

	const std::vector<Math::Vector3F>& AnimatorR::GetJointPositions() const
	{
		return jointPositions;
	}

	void AnimatorR::ClearJoints()
	{
		jointPositions.clear();
	}

	Math::Vector3F AnimatorR::ExtractJointPosition(const glm::mat4& transform)
	{
		return { transform[3][0], transform[3][1], transform[3][2] };
	}
}
