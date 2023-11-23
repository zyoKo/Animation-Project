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

		AssimpNodeData& GetRootNode();

		const std::map<std::string, BoneInfo>& GetBoneIDMap() const;

		decltype(auto) GetBindPose() const
		{
			return (bindPose);
		}

	private:
		std::vector<Bone> bones;

		AssimpNodeData rootNode;

		std::vector<std::pair<std::string, glm::mat4>> bindPose;

		std::map<std::string, BoneInfo> boneInfoMap;

		float duration;

		float ticksPerSecond;

		void ReadMissingBones(const aiAnimation* animation, Model& model);

		void ReadHierarchyData(AssimpNodeData& destinationNode, const aiNode* sourceNode, 
			AssimpNodeData* parent = nullptr);
	};
}
