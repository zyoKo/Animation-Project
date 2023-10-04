#include <AnimationPch.h>

#include "Model.h"

#include <assimp/postprocess.h>

#include "AssetManager/AssetManager.h"
#include "Core/Utilities/Utilites.h"

namespace Animator
{
	Model::Model(const std::string& path)
		:	gammaCorrection(false)
	{
		LoadModel(path);
	}

	void Model::Draw(const std::shared_ptr<Shader>& shader) const
	{
		for (const auto& mesh : meshes)
		{
			mesh.Draw(shader);
		}
	}

	const std::vector<Mesh>& Model::GetMeshes() const
	{
		return meshes;
	}

	void Model::SetDiffuseTextureForMeshes(const std::shared_ptr<ITexture2D>& textures)
	{
		for (auto& mesh : meshes)
		{
			mesh.AddTexture(textures);
		}
	}

	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;

		constexpr auto flags = aiProcess_Triangulate  | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices /* | aiProcess_FlipUVs */;

		const aiScene* scene = importer.ReadFile(path, flags);

		ANIM_ASSERT(!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode), "ERROR::ASSIMP::{0}", importer.GetErrorString());

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(const aiNode* aiNode, const aiScene* scene)
	{
		for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[aiNode->mMeshes[i]];
			meshes.emplace_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
		{
			ProcessNode(aiNode->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(const aiMesh* aiMesh, const aiScene* aiScene)
	{
		std::vector<Math::Vector3F> vertices;
		std::vector<Math::Vector3F> normals;
		std::vector<Math::Vector2F> texCoords;
		std::vector<Math::Vector3F> tangents;
		std::vector<Math::Vector3F> biTangents;
		std::vector<unsigned> indices;
		std::vector<BoneData> boneData;

		for (unsigned i = 0; i < aiMesh->mNumVertices; ++i)
		{
			vertices.push_back({ aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z });

			if (aiMesh->HasNormals())
			{
				normals.push_back({ aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z });
			}

			if (aiMesh->mTextureCoords[0])
			{
				texCoords.push_back({ aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y });
			}
			else
			{
				texCoords.push_back({ 0.0f, 0.0f });
			}

			if (aiMesh->HasTangentsAndBitangents())
			{
				tangents.push_back({ aiMesh->mTangents[i].x, aiMesh->mTangents[i].y, aiMesh->mTangents[i].z });

				biTangents.push_back({ aiMesh->mBitangents[i].x, aiMesh->mBitangents[i].y, aiMesh->mBitangents[i].z });
			}
		}

		for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i)
		{
			const aiFace face = aiMesh->mFaces[i];
			for (unsigned j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//aiMaterial* material = aiScene->mMaterials[aiMesh->mMaterialIndex];

		//std::vector<std::shared_ptr<ITexture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		boneData.resize(vertices.size());
		ExtractBoneWeightForVertices(boneData, aiMesh, aiScene, static_cast<unsigned>(vertices.size()));

		return { vertices, normals, texCoords, tangents, biTangents, boneData, indices };
	}

	std::map<std::string, BoneInfo>& Model::GetBoneInfoMap()
	{
		return boneInfoMap;
	}

	int& Model::GetBoneCount()
	{
		return boneCounter;
	}

	void Model::ExtractBoneWeightForVertices(std::vector<BoneData>& boneData, const aiMesh* aiMesh, const aiScene* aiScene, unsigned verticesSize)
	{
		for (unsigned boneIndex = 0; boneIndex < aiMesh->mNumBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = aiMesh->mBones[boneIndex]->mName.C_Str();
            if (!boneInfoMap.contains(boneName))
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = boneCounter;
                newBoneInfo.offset = Utils::AssimpGLMHelper::ConvertMatrixToGLMFormat(aiMesh->mBones[boneIndex]->mOffsetMatrix);
                boneInfoMap[boneName] = newBoneInfo;
                boneID = boneCounter;
                ++boneCounter;
            }
            else
            {
                boneID = boneInfoMap[boneName].id;
            }

			ANIM_ASSERT(boneID != -1, "Bone ID is not set right!");

            const auto weights = aiMesh->mBones[boneIndex]->mWeights;

            const unsigned numWeights = aiMesh->mBones[boneIndex]->mNumWeights;

            for (unsigned weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                const unsigned vertexId = weights[weightIndex].mVertexId;
                const float weight = weights[weightIndex].mWeight;

				ANIM_ASSERT(vertexId <= verticesSize, "VertexId can't be greater than the total size of Vertices!");

				boneData[vertexId].AddToBoneData(boneID, weight);
            }
        }
	}

	//std::vector<std::shared_ptr<ITexture2D>> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string textureName)
	//{
	//	std::vector<std::shared_ptr<ITexture2D>> textures;

	//	for(unsigned i = 0; i < material->GetTextureCount(type); ++i)
	//	{
	//		aiString str;
	//		material->GetTexture(type, i, &str);
	//		bool skip = false;
 //           for(unsigned int j = 0; j < textureLoaded.size(); j++)
 //           {
 //               if(std::strcmp(textureLoaded[j]->GetTextureName().data(), str.C_Str()) == 0)
 //               {
 //                   textures.push_back(textureLoaded[j]);
 //                   skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
 //                   break;
 //               }
 //           }
 //           if(!skip)
 //           {   // if texture hasn't been loaded already, load it
 //               Texture texture;
 //               texture.id = TextureFromFile(str.C_Str(), this->directory);
 //               texture.type = typeName;
 //               texture.path = str.C_Str();
 //               textures.push_back(texture);
 //               textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
 //           }
	//	}

	//	return textures;
	//}
}
