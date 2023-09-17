#pragma once

namespace Animator
{
	class IShader
	{
	public:
		virtual ~IShader() = 0;

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		
	};
}