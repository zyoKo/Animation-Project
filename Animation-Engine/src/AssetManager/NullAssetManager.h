#pragma once

#include "Interface/IAssetManager.h"

namespace AnimationEngine
{
	class NullAssetManager : public IAssetManager
	{
	public:
		std::weak_ptr<ITexture2D> CreateTexture(const std::string& filepath) override
		{
			return {};
		}

		std::weak_ptr<Shader> CreateShader(const std::string& shaderName, const std::string& vertexFilepath,
		                                   const std::string& fragmentFilepath) override
		{
			return {};
		}

		std::weak_ptr<ITexture2D> RetrieveTextureFromStorage(const std::string& textureName) override
		{
			return {};
		}

		std::weak_ptr<Shader> RetrieveShaderFromStorage(const std::string& shaderName) override
		{
			return {};
		}

		void ClearStores() override
		{
		}
	};
}
