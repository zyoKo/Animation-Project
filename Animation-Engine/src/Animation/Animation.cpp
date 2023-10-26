#include <AnimationPch.h>

#include <assimp/postprocess.h>

#include "Animation.h"

#include "Core/Utilities/Utilites.h"

namespace AnimationEngine
{
	Animation::Animation(const std::string& animationPath, Model* model)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);

		ANIM_ASSERT(scene && scene->mRootNode, "Scene is empty or Scene's Root Node is empty!\nAssimp Error: {0}", importer.GetErrorString());

		const auto animation = scene->mAnimations[0];
		duration = static_cast<float>(animation->mDuration);
		ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);

		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();

		ReadHierarchyData(rootNode, scene->mRootNode);

		ReadMissingBones(animation, *model);
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		auto iter = std::find_if(bones.begin(), bones.end(), 
			[&](const Bone& bone)
			{
				return bone.GetBoneName() == name;
			}
		);

		if (iter == bones.end())
			return nullptr;

		return &(*iter);
	}

	float Animation::GetDuration() const
	{
		return duration;
	}

	float Animation::GetTicksPerSecond() const
	{
		return ticksPerSecond;
	}

	const AssimpNodeData& Animation::GetRootNode() const
	{
		return rootNode;
	}

	const std::map<std::string, BoneInfo>& Animation::GetBoneIDMap() const
	{
		return boneInfoMap;
	}

	void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		auto size = static_cast<int>(animation->mNumChannels);

		auto& boneInfoMap = model.GetBoneInfoMap();
		int& boneCount = model.GetBoneCount();

		for (int i = 0; i < size; ++i)
		{
			const auto channel = animation->mChannels[i];

			std::string boneName = channel->mNodeName.data;

			if (!boneInfoMap.contains(boneName))
			{
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
			}

			bones.emplace_back(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel);
		}

		this->boneInfoMap = boneInfoMap;
	}

	void Animation::ReadHierarchyData(AssimpNodeData& destinationNode, const aiNode* sourceNode)
	{
		ANIM_ASSERT(sourceNode, "Source Node is Empty!");

		destinationNode.name = sourceNode->mName.data;
		destinationNode.transformation = Utils::AssimpToGLMHelper::ConvertMatrixToGLMFormat(sourceNode->mTransformation);
		destinationNode.childrenCount = sourceNode->mNumChildren;

		for (int i = 0; i < sourceNode->mNumChildren; ++i)
		{
			AssimpNodeData newAssimpNodeData;
			ReadHierarchyData(newAssimpNodeData, sourceNode->mChildren[i]);
			destinationNode.children.push_back(newAssimpNodeData);
		}
	}
}
