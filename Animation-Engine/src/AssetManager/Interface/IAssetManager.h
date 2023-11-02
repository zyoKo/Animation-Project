#pragma once

namespace AnimationEngine
{
	class ITexture2D;

	class Shader;
}

namespace AnimationEngine
{
	class IAssetManager
	{
	public:
		virtual ~IAssetManager() = default;

		virtual std::shared_ptr<ITexture2D> CreateTexture(const std::string& filepath) = 0;

		virtual std::shared_ptr<Shader> CreateShader(
			const std::string& shaderName, 
			const std::string& vertexFilepath, const std::string& fragmentFilepath) = 0;

		virtual std::shared_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName) = 0;

		virtual std::shared_ptr<Shader> RetrieveShaderFromStorage(const std::string& shaderName) = 0;

		virtual void ClearStores() = 0;
	};
}