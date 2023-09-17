#pragma once

#include "Graphics/OpenGL/Interfaces/IShader.h"

namespace Animator
{
	class Shader : public IShader
	{
	public:
		Shader() = delete;

		Shader(const std::string& shaderName, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		~Shader() override;
	};
}
