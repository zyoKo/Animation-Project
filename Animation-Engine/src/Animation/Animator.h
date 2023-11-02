#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace AnimationEngine
{
	struct AssimpNodeData;

	class Animation;

	class Animator
	{
	public:
		Animator();

		Animator(Animation* animation);

		~Animator() = default;

		void ChangeAnimation(Animation* newAnimation);

		void UpdateAnimation();

		void PlayAnimation(Animation* animation);

		void CalculateBoneTransformWithVQS(const AssimpNodeData* node, Math::VQS parentVQS);

		const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

		const std::vector<Math::Vector3F>& GetJointPositions() const;

		void ClearJoints();

	private:
		std::vector<glm::mat4> finalBoneMatrices;

		std::vector<Math::Vector3F> jointPositions;

		Animation* currentAnimation;

		float currentTime;

		Math::Vector3F ExtractJointPosition(const glm::mat4& transform);
	};
}