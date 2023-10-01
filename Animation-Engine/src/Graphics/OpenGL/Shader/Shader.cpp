#include <AnimationPch.h>

#include "Shader.h"

#include "Core/Logger/Log.h"
#include "Core/Logger/GLDebug.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Animator
{
	Shader::Shader(const std::string& shaderName, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
		:	shaderID(0),
			shaderName(shaderName)
	{
		const unsigned int vertexShaderId	= CompileShaderSource(vertexShaderSource,	GL_VERTEX_SHADER);
		const unsigned int fragmentShaderId = CompileShaderSource(fragmentShaderSource, GL_FRAGMENT_SHADER);

		shaderID = GL_CALL(glCreateProgram);

		GL_CALL(glAttachShader, shaderID, vertexShaderId);
		GL_CALL(glAttachShader, shaderID, fragmentShaderId);
		GL_CALL(glLinkProgram, shaderID);

		ShaderErrorChecker(shaderID, ShaderErrorType::LINKER);

		GL_CALL(glDeleteShader, vertexShaderId);
		GL_CALL(glDeleteShader, fragmentShaderId);
	}

	Shader::~Shader()
	{
		GL_CALL(glUseProgram, 0);

		GL_CALL(glDeleteProgram, shaderID);
	}

	void Shader::Bind() const
	{
		GL_CALL(glUseProgram, shaderID);
	}

	void Shader::UnBind() const
	{
		GL_CALL(glUseProgram, 0);
	}

	unsigned Shader::GetShaderID() const
	{
		return shaderID;
	}

	void Shader::SetShaderName(const std::string& name)
	{
		this->shaderName = name;
	}

	void Shader::SetUniformMatrix4F(const glm::mat4& mat4, const std::string& uniformName)
	{
		GL_CALL(glUniformMatrix4fv, GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void Shader::SetUniformMatrix3F(const glm::mat3& mat3, const std::string& uniformName)
	{
		GL_CALL(glUniformMatrix3fv, GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void Shader::SetUniformVector4F(const Math::Vector4F& vec4, const std::string& uniformName)
	{
		GL_CALL(glUniform4fv, GetUniformLocation(uniformName), 1, vec4.GetPointerToData());
	}

	void Shader::SetUniformVector3F(const Math::Vector3F& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3fv, GetUniformLocation(uniformName), 1, vec3.GetPointerToData());
	}

	void Shader::SetUniformVector3UI(const Math::Vector3UI& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3uiv, GetUniformLocation(uniformName), 1, vec3.GetPointerToData());
	}

	void Shader::SetUniformVector2F(const Math::Vector2F& vec2, const std::string& uniformName)
	{
		GL_CALL(glUniform2fv, GetUniformLocation(uniformName), 1, vec2.GetPointerToData());
	}

	void Shader::SetUniformInt(int value, const std::string& uniformName)
	{
		GL_CALL(glUniform1i, GetUniformLocation(uniformName), value);
	}

	void Shader::SetUniformFloat(float value, const std::string& uniformName)
	{
		GL_CALL(glUniform1f, GetUniformLocation(uniformName), value);
	}

	unsigned Shader::CompileShaderSource(const std::string& shaderSource, GLenum shaderType)
	{
		const char* rawShaderSource = shaderSource.c_str();
		const unsigned int shaderId = GL_CALL(glCreateShader, shaderType);
		GL_CALL(glShaderSource, shaderId, 1, &rawShaderSource, nullptr);
		GL_CALL(glCompileShader, shaderId);

		ShaderErrorChecker(shaderId, ShaderErrorType::COMPILER);

		return shaderId;
	}

	void Shader::ShaderErrorChecker(unsigned int shaderId, ShaderErrorType errorType)
	{
		int compilationSuccessful;
		switch(errorType)
		{
		case ShaderErrorType::COMPILER:
			GL_CALL(glGetShaderiv, shaderId, GL_COMPILE_STATUS, &compilationSuccessful);
			break;

		case ShaderErrorType::LINKER:
			GL_CALL(glGetProgramiv, shaderId, GL_LINK_STATUS, &compilationSuccessful);
			break;

		case ShaderErrorType::NONE:
			LOG_INFO("Please specify Shader Error Type for the shader error checking");
			return;
		}

		if (!compilationSuccessful)
		{
			// Two step process to get log message
			int errorBufferLength;
			GL_CALL(glGetProgramiv, shaderId, GL_INFO_LOG_LENGTH, &errorBufferLength);
			std::vector<char> errorMessage(errorBufferLength);
			GL_CALL(glGetProgramInfoLog, shaderId, errorBufferLength, nullptr, errorMessage.data());

			LOG_ERROR(errorMessage.data());
		}
	}

	int Shader::GetUniformLocation(const std::string& uniformName)
	{
		// Better Optimization
		if (uniformLocationCache.contains(uniformName))
			return uniformLocationCache[uniformName];

		const int location = glGetUniformLocation(shaderID, uniformName.c_str());
		if (location == -1) 
		{
			LOG_WARN("Warning: Uniform '{0}' does not exists!", uniformName);
			std::cout << "Warning: uniform '" << uniformName << "' doesn't exists!" << std::endl;
		}

		uniformLocationCache[uniformName] = location;
		return location;
	}
}
