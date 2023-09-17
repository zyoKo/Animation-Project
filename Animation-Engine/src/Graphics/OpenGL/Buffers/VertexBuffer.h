#pragma once

#include "Graphics/OpenGL/Interfaces/IVertexBuffer.h"

namespace Animator
{
	class VertexBuffer : public IVertexBuffer
	{
	public:
		VertexBuffer();

		VertexBuffer(const void* bufferData, unsigned int bufferSize);

		~VertexBuffer() override;

		void Bind() override;

		void UnBind() override;

		void SetSize(unsigned int bufferSize) override;

		void SetData(const void* bufferData) const override;

		//void SetVertexBufferLayout(const )

	private:
		unsigned int bufferID;

		unsigned int bufferDataSize;

		// VertexBufferLayout object here
	};
}
