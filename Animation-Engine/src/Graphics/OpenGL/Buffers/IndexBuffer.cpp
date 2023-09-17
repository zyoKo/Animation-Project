#include "AnimationPch.h"

#include "IndexBuffer.h"

#include "Core/Logger/GLDebug.h"
#include "glad/glad.h"

namespace Animator
{
	IndexBuffer::IndexBuffer()
		:	indexBufferDataSize(0)
	{
		GL_CALL(glCreateBuffers, 1, &bufferID);
	}

	IndexBuffer::IndexBuffer(const void* bufferData, unsigned bufferSize)
		:	indexBufferDataSize(bufferSize)
	{
		GL_CALL(glCreateBuffers, 1, &bufferID);
		GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, bufferID);
		GL_CALL(glNamedBufferData, GL_ELEMENT_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &bufferID);
	}

	void IndexBuffer::Bind()
	{
		GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, bufferID);
	}

	void IndexBuffer::UnBind()
	{
		GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetSize(unsigned int bufferSize)
	{
		this->indexBufferDataSize = bufferSize;
	}

	void IndexBuffer::SetData(const void* bufferData) const
	{
		GL_CALL(glNamedBufferSubData, bufferID, 0, indexBufferDataSize, bufferData);
	}
}
