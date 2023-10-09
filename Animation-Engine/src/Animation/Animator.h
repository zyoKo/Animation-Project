#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Animator
{
	struct AssimpNodeData;

	class Animation;

	class AnimatorR
	{
	public:
		AnimatorR();

		AnimatorR(Animation* animation);

		~AnimatorR() = default;

		void ChangeAnimation(Animation* newAnimation);

		void UpdateAnimation(float dt);

		void PlayAnimation(Animation* animation);

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

		const std::vector<Math::Vector3F>& GetJointPositions() const
		{
			return jointPositions;
		}

		void ClearJoints()
		{
			jointPositions.clear();
		}

	private:
		std::vector<glm::mat4> finalBoneMatrices;

		std::vector<Math::Vector3F> jointPositions;

		Animation* currentAnimation;

		float currentTime;

		float deltaTime;

		Math::Vector3F ExtractJointPosition(const glm::mat4& transform)
		{
			return { transform[3][0], transform[3][1], transform[3][2] };
		}
	};
}