#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
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

	private:
		std::vector<Mesh> meshes;

		bool gammaCorrection;

		void LoadModel(const std::string& path);

		void ProcessNode(aiNode* node, const aiScene* scene);

		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		//std::vector<std::shared_ptr<ITexture2D>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string textureName);
	};
}
