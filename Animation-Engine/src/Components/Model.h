#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Animation/DataTypes/BoneInfo.h"
#include "Graphics/OpenGL/Textures/ITexture2D.h"

namespace Animator
{
	class Shader;

	class Model
	{
	public:
		Model(const std::string& path);

		void Draw(const std::shared_ptr<Shader>& shader) const;

		const std::vector<Mesh>& GetMeshes() const;

		void SetDiffuseTextureForMeshes(const std::shared_ptr<ITexture2D>& textures);

		std::map<std::string, BoneInfo>& GetBoneInfoMap();

		int& GetBoneCount();

	private:
		std::vector<Mesh> meshes;

		bool gammaCorrection;

		std::map<std::string, BoneInfo> boneInfoMap;

		int boneCounter = 0;

		void LoadModel(const std::string& path);

		void ProcessNode(const aiNode* aiNode, const aiScene* scene);

		Mesh ProcessMesh(const aiMesh* aiMesh, const aiScene* aiScene);

		void ExtractBoneWeightForVertices(std::vector<BoneData>& boneData, const aiMesh* aiMesh, const aiScene* aiScene, unsigned verticesSize);

		//std::vector<std::shared_ptr<ITexture2D>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string textureName);
	};
}
