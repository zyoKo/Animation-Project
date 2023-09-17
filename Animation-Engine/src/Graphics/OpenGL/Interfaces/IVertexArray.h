#pragma once

namespace Animator
{
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;
	};
}