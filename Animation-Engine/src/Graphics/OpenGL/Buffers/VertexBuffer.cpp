#include "AnimationPch.h"

#include "VertexBuffer.h"

#include "Core/Logger/GLDebug.h"

namespace Animator
{
	VertexBuffer::VertexBuffer()
		:	bufferDataSize(0)
	{
		glCreateBuffers(1, &bufferID);
	}

	VertexBuffer::VertexBuffer(const void* bufferData, unsigned int bufferSize)
	{
		this->bufferDataSize = bufferSize;
		GL_CALL(glCreateBuffers, 1, &bufferID);
		GL_CALL(glNamedBufferData, bufferID, bufferSize, bufferData, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &bufferID);
	}

	unsigned VertexBuffer::GetBufferID() const
	{
		return bufferID;
	}

	void VertexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, bufferID);
	}

	void VertexBuffer::UnBind() const
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetSize(unsigned int bufferSize)
	{
		this->bufferDataSize = bufferSize;
	}

	void VertexBuffer::SetData(const void* bufferData) const
	{
		GL_CALL(glNamedBufferData, bufferID, bufferDataSize, bufferData, GL_STATIC_DRAW);
	}

	void VertexBuffer::SetVertexBufferLayout(const VertexBufferLayout& layout)
	{
		vertexBufferLayout = layout;
	}

	VertexBufferLayout& VertexBuffer::GetVertexBufferLayout()
	{
		return vertexBufferLayout;
	}
}
