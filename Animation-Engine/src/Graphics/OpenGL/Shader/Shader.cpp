#include <AnimationPch.h>

#include "Shader.h"

#include "Core/Logger/Log.h"
#include "Core/Logger/GLDebug.h"

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

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	Shader::~Shader()
	{
		GL_CALL(glUseProgram, 0);

		GL_CALL(glDeleteProgram, shaderID);
	}

	void Shader::Bind() const
	{
		glUseProgram(shaderID);
	}

	void Shader::UnBind() const
	{
		glUseProgram(0);
	}

	unsigned Shader::GetShaderID() const
	{
		return shaderID;
	}

	void Shader::SetShaderName(const std::string& name)
	{
		this->shaderName = name;
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
		const char* acceptableShaderSource = shaderSource.c_str();
		const unsigned int shaderId = GL_CALL(glCreateShader, shaderType);
		GL_CALL(glShaderSource, shaderId, 1, &acceptableShaderSource, NULL);
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
			GL_CALL(glGetProgramiv, shaderId, GL_COMPILE_STATUS, &compilationSuccessful);
			break;

		case ShaderErrorType::LINKER:
			GL_CALL(glGetProgramiv, shaderId, GL_LINK_STATUS, &compilationSuccessful);
			break;

		case ShaderErrorType::NONE:
			LOG_INFO("Please specify Shader Error Type for the shader error checking");
			return;
		}

		if (compilationSuccessful)
			return;

		// Two step process to get log message
		int errorBufferLength;
		GL_CALL(glGetProgramiv, shaderId, GL_INFO_LOG_LENGTH, &errorBufferLength);
		std::vector<char> errorMessage(errorBufferLength);
		GL_CALL(glGetProgramInfoLog, shaderId, errorBufferLength, nullptr, errorMessage.data());

		LOG_ERROR(errorMessage.data());
	}

	int Shader::GetUniformLocation(const std::string& uniformName)
	{
		// Better Optimization
		if (uniformLocationCache.find(uniformName) != uniformLocationCache.end())
			return uniformLocationCache[uniformName];

		const int location = GL_CALL(glGetUniformLocation(shaderID, uniformName.c_str()));

		if (location == -1) 
		{
			LOG_WARN("Warning: Uniform '{0}' does not exists!", uniformName);
			std::cout << "Warning: uniform '" << uniformName << "' doesn't exists!" << std::endl;
		}

		uniformLocationCache[uniformName] = location;
		return location;
	}
}
