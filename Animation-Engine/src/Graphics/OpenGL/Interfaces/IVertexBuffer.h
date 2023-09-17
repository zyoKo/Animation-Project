#pragma once

namespace Animator
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual void SetSize(unsigned int bufferSize) = 0;

		virtual void SetData(const void* bufferData) const = 0;
	};
}