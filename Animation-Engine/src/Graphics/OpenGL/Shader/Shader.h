#pragma once

#include <unordered_map>

#include "glad/glad.h"

#include "Interface/IShader.h"

#include <glm/mat3x3.hpp>

namespace AnimationEngine
{
	enum class ShaderErrorType
	{
		NONE = -1,

		COMPILER = 0,

		LINKER = 1
	};

	class Shader : public IShader
	{
	public:
		Shader(const std::string& shaderName, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		~Shader() override;

		void Bind() const override;

		void UnBind() const override;

		unsigned GetShaderID() const override;

		void SetShaderName(const std::string& name) override;

		void SetUniformMatrix4F(const glm::mat4& mat4, const std::string& uniformName) override;

		void SetUniformMatrix3F(const glm::mat3& mat3, const std::string& uniformName) override;

		void SetUniformVector4F(const Math::Vector4F& vec4, const std::string& uniformName) override;

		void SetUniformVector3F(const Math::Vector3F& vec3, const std::string& uniformName) override;

		void SetUniformVector3UI(const Math::Vector3UI& vec3, const std::string& uniformName) override;

		void SetUniformVector2F(const Math::Vector2F& vec2, const std::string& uniformName) override;

		void SetUniformInt(int value, const std::string& uniformName) override;

		void SetUniformFloat(float value, const std::string& uniformName) override;

	private:
		unsigned int shaderID;

		std::string shaderName;

		std::unordered_map<std::string, int> uniformLocationCache;

		unsigned int CompileShaderSource(const std::string& shaderSource, GLenum shaderType);

		void ShaderErrorChecker(unsigned int shaderId, ShaderErrorType errorType);

		int GetUniformLocation(const std::string& uniformName);
	};
}
