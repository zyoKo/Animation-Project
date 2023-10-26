#pragma once

#include <assimp/anim.h>

#include <glm/matrix.hpp>

#include "DataTypes/BoneData.h"

namespace AnimationEngine
{
	class Bone
	{
	public:
		Bone(std::string name, int boneID, const aiNodeAnim* channel);

		void Update(float animationTime);

		const std::string& GetBoneName() const;

		int GetBoneID() const;

		const glm::mat4& GetLocalTransform() const;

		const Math::VQS& GetLocalVQS() const;

		int GetPositionIndexAt(float time) const;

		int GetRotationIndexAt(float time) const;

		int GetScalingIndexAt(float time) const;

	private:
		std::string name;

		int boneID;

		glm::mat4 localTransform;

		Math::VQS localVQS;

		std::vector<KeyPosition> keyPositions;

		std::vector<KeyRotation> keyRotations;

		std::vector<KeyScale> keyScales;

		int numPositions;

		int numRotations;

		int numScales;

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		glm::mat4 InterpolationPosition(float animationTime);

		glm::mat4 InterpolationRotation(float animationTime);

		glm::mat4 InterpolationScaling(float animationTime);

		Math::VQS InterpolateWithVQS(float animationTime);
	};
}
