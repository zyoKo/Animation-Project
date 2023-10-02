#include <AnimationPch.h>

#include "Model.h"

#include "AssetManager/AssetManager.h"

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

		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		ANIM_ASSERT(!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode), "ERROR::ASSIMP::{0}", importer.GetErrorString());

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Math::Vector3F> vertices;
		std::vector<Math::Vector3F> normals;
		std::vector<Math::Vector2F> texCoords;
		std::vector<Math::Vector3F> tangents;
		std::vector<Math::Vector3F> biTangents;
		std::vector<unsigned> indices;

		for (unsigned i = 0; i < mesh->mNumVertices; ++i)
		{
			vertices.push_back({ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

			if (mesh->HasNormals())
			{
				normals.push_back({ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
			}

			if (mesh->mTextureCoords[0])
			{
				texCoords.push_back({ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
			}
			else
			{
				texCoords.push_back({ 0.0f, 0.0f });
			}

			if (mesh->HasTangentsAndBitangents())
			{
				tangents.push_back({ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z });

				biTangents.push_back({ mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z });
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//std::vector<std::shared_ptr<ITexture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		//Mesh tempMesh(vertices, normals, texCoords, tangents, biTangents, indices);
		//tempMesh.AddTexture(texture);

		return { vertices, normals, texCoords, tangents, biTangents, indices };
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
