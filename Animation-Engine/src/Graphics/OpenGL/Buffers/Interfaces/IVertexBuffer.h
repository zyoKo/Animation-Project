#pragma once

#include "Graphics/OpenGL/Buffers/Structure/VertexBufferLayout.h"

namespace Animator
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual unsigned int GetBufferID() const = 0;

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		virtual void SetSize(unsigned int bufferSize) = 0;

		virtual void SetData(const void* bufferData) const = 0;

		virtual void SetVertexBufferLayout(const VertexBufferLayout& layout) = 0;

		virtual VertexBufferLayout& GetVertexBufferLayout() = 0;
	};
}
