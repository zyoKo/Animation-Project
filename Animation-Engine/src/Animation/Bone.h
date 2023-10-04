#pragma once

#include <assimp/anim.h>

#include <glm/matrix.hpp>

#include "DataTypes/BoneData.h"

namespace Animator
{
	class Bone
	{
	public:
		Bone(std::string name, int boneID, const aiNodeAnim* channel);

		void Update(float animationTime);

		const std::string& GetBoneName() const;

		int GetBoneID() const;

		const glm::mat4& GetLocalTransform() const;

		int GetPositionIndex(float animationTime) const;

		int GetRotationIndex(float animationTime) const;

		int GetScalingIndex(float animationTime) const;

	private:
		std::string name;

		int boneID;

		glm::mat4 localTransform;

		std::vector<KeyPosition> positions;

		std::vector<KeyRotation> rotations;

		std::vector<KeyScale> scales;

		int numPositions;

		int numRotations;

		int numScales;

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		glm::mat4 InterpolationPosition(float animationTime);

		glm::mat4 InterpolationRotation(float animationTime);

		glm::mat4 InterpolationScaling(float animationTime);
	};
}
