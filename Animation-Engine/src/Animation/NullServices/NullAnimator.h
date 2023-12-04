#pragma once

#include "Animation/Interface/IAnimator.h"

namespace AnimationEngine
{
	class NullAnimator : public IAnimator
	{
		void ChangeAnimation(Animation* animation) override;

		void UpdateAnimation() override;

		void PlayAnimation(Animation* animation) override;

		void ResetAnimation() override;

		void CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS) override;

		const std::vector<glm::mat4>& GetFinalBoneMatrices() const override;

		const std::vector<Math::Vector3F>& GetJointPositions() const override;

		float GetAnimationSpeedFactor() const override;

		void SetAnimationSpeedFactor(float value) override;

		void ClearJoints() override;
	};
}
