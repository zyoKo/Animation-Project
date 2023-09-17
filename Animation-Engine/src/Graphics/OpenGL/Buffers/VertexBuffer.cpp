#include "AnimationPch.h"

#include "VertexBuffer.h"

#include "Core/Logger/GLDebug.h"

namespace Animator
{
	VertexBuffer::VertexBuffer()
		:	bufferDataSize(0)
	{
		GL_CALL(glCreateBuffers, 1, &bufferID);
	}

	VertexBuffer::VertexBuffer(const void* bufferData, unsigned int bufferSize)
	{
		this->bufferDataSize = bufferSize;
		GL_CALL(glCreateBuffers, 1, &bufferID);
		GL_CALL(glNamedBufferData, GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, bufferID);
	}

	VertexBuffer::~VertexBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &bufferID);
	}

	void VertexBuffer::Bind()
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, bufferID);
	}

	void VertexBuffer::UnBind()
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetSize(unsigned int bufferSize)
	{
		this->bufferDataSize = bufferSize;
	}

	void VertexBuffer::SetData(const void* bufferData) const
	{
		GL_CALL(glNamedBufferSubData, bufferID, 0, bufferDataSize, bufferData);
	}
}
