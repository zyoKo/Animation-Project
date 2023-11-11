#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Interface/IAnimator.h"

namespace AnimationEngine
{
	struct AssimpNodeData;

	class Animation;

	class Animator : public IAnimator
	{
	public:
		Animator();

		Animator(Animation* animation);

		void ChangeAnimation(Animation* newAnimation) override;

		void UpdateAnimation() override;

		void PlayAnimation(Animation* animation) override;

		void ResetAnimation() override;

		void CalculateBoneTransformWithVQS(const AssimpNodeData* node, Math::VQS parentVQS) override;

		const std::vector<glm::mat4>& GetFinalBoneMatrices() const override;

		const std::vector<Math::Vector3F>& GetJointPositions() const override;

		float GetAnimationSpeedFactor() const override;

		void SetAnimationSpeedFactor(float value) override;

		void ClearJoints() override;

	private:
		std::vector<glm::mat4> finalBoneMatrices;

		std::vector<Math::Vector3F> jointPositions;

		Animation* currentAnimation;

		float currentTime;

		float animationSpeedFactor;

		void ExtractParentJointAndChildJoints(const Math::VQS& parent, const Math::VQS& child);
	};
}
