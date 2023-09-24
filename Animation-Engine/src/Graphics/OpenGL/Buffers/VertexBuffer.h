#pragma once

#include "Interfaces/IVertexBuffer.h"

namespace Animator
{
	class VertexBuffer : public IVertexBuffer
	{
	public:
		VertexBuffer();

		VertexBuffer(const void* bufferData, unsigned int bufferSize);

		~VertexBuffer() override;

		unsigned int GetBufferID() const override;

		void Bind() const override;

		void UnBind() const override;

		void SetSize(unsigned int bufferSize) override;

		void SetData(const void* bufferData) const override;

		void SetVertexBufferLayout(const VertexBufferLayout& layout) override;

		VertexBufferLayout& GetVertexBufferLayout() override;

	private:
		unsigned int bufferID;

		unsigned int bufferDataSize;

		VertexBufferLayout vertexBufferLayout;
	};
}
