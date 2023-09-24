#pragma once

#include "Math/Math.h"

namespace Animator
{
	class IShader
	{
	public:
		virtual ~IShader() = 0;

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		virtual unsigned int GetShaderID() const = 0;

		//virtual void SetUniformMat4F(const Math::Mat4F& mat4, const std::string& uniformName) = 0;

		//virtual void SetUniformVector4F(const Math::Vector4F& vec4, const std::string& uniformName) = 0;

		virtual void SetUniformVector3F(const Math::Vector3F& vec3, const std::string& uniformName) = 0;

		virtual void SetUniformVector3UI(const Math::Vector3UI& vec3, const std::string& uniformName) = 0;

		virtual void SetUniformVector2F(const Math::Vector2F& vec2, const std::string& uniformName) = 0;

		virtual void SetUniformInt(int value, const std::string& uniformName) = 0;

		virtual void SetUniformFloat(float value, const std::string& uniformName) = 0;

		virtual void SetShaderName(const std::string& shaderName) = 0;
	};
}
