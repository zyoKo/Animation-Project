#pragma once

#include "Interface/IAssetManager.h"

namespace AnimationEngine
{
	class NullAssetManager : public IAssetManager
	{
	public:
		std::shared_ptr<ITexture2D> CreateTexture(const std::string& filepath) override
		{
			return nullptr;
		}

		std::shared_ptr<Shader> CreateShader(const std::string& shaderName, const std::string& vertexFilepath,
			const std::string& fragmentFilepath) override
		{
			return nullptr;
		}

		std::shared_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName) override
		{
			return nullptr;
		}

		std::shared_ptr<Shader> RetrieveShaderFromStorage(const std::string& shaderName) override
		{
			return nullptr;
		}

		void ClearStores() override
		{
		}
	};
}
