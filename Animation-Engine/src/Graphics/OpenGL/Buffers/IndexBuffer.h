#pragma once

#include "Graphics/OpenGL/Interfaces/IIndexBuffer.h"

namespace Animator
{
	class IndexBuffer : public IIndexBuffer
	{
	public:
		IndexBuffer();

		IndexBuffer(const void* bufferData, unsigned int bufferSize);

		~IndexBuffer() override;

		void Bind() override;

		void UnBind() override;

		void SetSize(unsigned int bufferSize) override;

		void SetData(const void* bufferData) const override;

	private:
		unsigned int bufferID;

		unsigned int indexBufferDataSize;
	};
}
