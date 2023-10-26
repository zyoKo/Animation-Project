#pragma once

#include "Bone.h"
#include "Animation/Model.h"
#include "DataTypes/AssimpNodeData.h"

#include <map>

namespace AnimationEngine
{
	class Animation
	{
	public:
		Animation() = default;

		~Animation() = default;

		Animation(const std::string& animationPath, Model* model);

		Bone* FindBone(const std::string& name);

		float GetDuration() const;

		float GetTicksPerSecond() const;

		const AssimpNodeData& GetRootNode() const;

		const std::map<std::string, BoneInfo>& GetBoneIDMap() const;

	private:
		void ReadMissingBones(const aiAnimation* animation, Model& model);

		void ReadHierarchyData(AssimpNodeData& destinationNode, const aiNode* sourceNode);

		float duration;

		float ticksPerSecond;

		std::vector<Bone> bones;

		AssimpNodeData rootNode;

		std::map<std::string, BoneInfo> boneInfoMap;
	};
}
