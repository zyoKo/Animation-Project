#include <AnimationPch.h>

#include "Animator.h"

#include <glm/gtx/string_cast.hpp>

#include "Animation/Animation.h"

#include "DataTypes/AssimpNodeData.h"

namespace Animator
{
	AnimatorR::AnimatorR(Animation* animation)
		:	currentAnimation(animation),
			currentTime(0.0f),
			deltaTime(0.0f)
	{
		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
			finalBoneMatrices.emplace_back(1.0f);
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
		glm::mat4 nodeTransform = node->transformation;

		//LOG_CRITICAL("===================================\n");

		Bone* bone = currentAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(currentTime);
			nodeTransform = bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		//LOG_INFO("NODE TRANSFORM\n" + glm::to_string(nodeTransform) + "\n");
		//LOG_INFO("PARENT TRANSFORM\n" + glm::to_string(parentTransform) + "\n");

		//LOG_WARN("GLOBAL TRANSFORM\n" + glm::to_string(globalTransformation) + "\n");

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.contains(nodeName))
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;

			//LOG_TRACE("OFFSET\n" + glm::to_string(offset) + "\n");

			finalBoneMatrices[index] = globalTransformation * offset;
			//LOG_WARN("FINAL BONE MATRIX[" + std::to_string(index) + "]\n" + glm::to_string(globalTransformation) + "\n");
		}

		//LOG_CRITICAL("===================================\n");

		for (int i = 0; i < node->childrenCount; ++i)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}

	const std::vector<glm::mat4>& AnimatorR::GetFinalBoneMatrices() const
	{
		return finalBoneMatrices;
	}
}
