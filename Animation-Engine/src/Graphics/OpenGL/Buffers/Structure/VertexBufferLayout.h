#pragma once

#include <vector>

#include "VertexBufferElements.h"

namespace Animator
{
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			:	stride(0)
		{
		}

		VertexBufferLayout(const VertexBufferLayout& vertexBufferLayout) = delete;

		VertexBufferLayout& operator=(const VertexBufferLayout& vertexBufferLayout)
		{
			bufferElements = vertexBufferLayout.bufferElements;
			stride = vertexBufferLayout.stride;
			return *this;
		}

		~VertexBufferLayout() = default;

		unsigned int GetStride() const
		{
			return stride;
		}

		const std::vector<VertexBufferElements>& GetVertexBufferElements() const
		{
			return bufferElements;
		}

		template<typename T>
		void Push(unsigned int count)
		{
			//ANIM_ASSERT(false, "Please provide a type!");
		}

		template<>
		void Push<float>(unsigned int count)
		{
			bufferElements.push_back({ GL_FLOAT, count, GL_FALSE });
			stride += VertexBufferElements::GetSizeofType(GL_FLOAT) * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			bufferElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			stride += VertexBufferElements::GetSizeofType(GL_UNSIGNED_INT) * count;
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			bufferElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			stride += VertexBufferElements::GetSizeofType(GL_UNSIGNED_BYTE) * count;
		}

	private:
		std::vector<VertexBufferElements> bufferElements;

		unsigned int stride;
	};
}
