#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Animator
{
	struct AssimpNodeData;

	class Animation;

	class AnimatorR
	{
	public:
		AnimatorR(Animation* animation);

		~AnimatorR() = default;

		void UpdateAnimation(float dt);

		void PlayAnimation(Animation* animation);

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

	private:
		std::vector<glm::mat4> finalBoneMatrices;

		Animation* currentAnimation;

		float currentTime;

		float deltaTime;
	};
}