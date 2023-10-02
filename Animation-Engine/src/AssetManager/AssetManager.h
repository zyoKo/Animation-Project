#pragma once

#include "AssetStore/AssetStore.h"

#include "Graphics/RenderApi.h"

namespace Animator
{
	using stbi_uc = unsigned char;

	class AssetManager
	{
	public:
		std::shared_ptr<ITexture2D> CreateTexture(const std::string& filepath);

		std::shared_ptr<Shader> CreateShader(const std::string& shaderName, const std::string& vertexFilepath, const std::string& fragmentFilepath);

		std::shared_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName);

		std::shared_ptr<Shader> RetrieveShaderFromStorage(const std::string& shaderName);

		void ClearStores();

	private:
		AssetStore<ITexture2D> textureStore;

		AssetStore<Shader> shaderStore;

		static stbi_uc* LoadTexture(const std::string& textureFile, int* width, int* height, int* depth);

		static std::string ReadShaderFile(const std::string& filepath);
	};
}
