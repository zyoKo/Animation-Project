#include "AnimationPch.h"

#include "VertexBuffer.h"

#include "Core/Logger/GLDebug.h"

namespace Animator
{
	VertexBuffer::VertexBuffer()
		:	vertexBufferDataSize(0)
	{
		GL_CALL(glCreateBuffers, 1, &bufferID);
	}

	VertexBuffer::VertexBuffer(const void* bufferData, unsigned int bufferSize)
	{
		this->vertexBufferDataSize = bufferSize;
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
		this->vertexBufferDataSize = bufferSize;
		GL_CALL(glNamedBufferData, bufferID, bufferSize, nullptr, GL_STATIC_DRAW);
	}

	// Offset is 0
	void VertexBuffer::SetData(const void* bufferData) const
	{
		GL_CALL(glNamedBufferSubData, bufferID, 0, vertexBufferDataSize, bufferData);
	}

	void VertexBuffer::SetVertexBufferLayout(const VertexBufferLayout& layout)
	{
		vertexBufferLayout = layout;
	}

	VertexBufferLayout& VertexBuffer::GetVertexBufferLayout()
	{
		return vertexBufferLayout;
	}

	// Custom Offset
	void VertexBuffer::OverwriteVertexBufferData(unsigned int layoutLocation, const void* bufferData, unsigned int bufferSize)
	{
		const auto element = vertexBufferLayout.GetVertexBufferElements()[layoutLocation];

		unsigned int offset = element.offset;
		const auto bytesToRead = GetSizeofCustomType(element.type);

		const char* bufferDataStartPointer = static_cast<const char*>(bufferData);
		const char* bufferDataPointerEnd = bufferDataStartPointer + bufferSize;

		while (bufferDataStartPointer < bufferDataPointerEnd)
		{
			GL_CALL(glNamedBufferSubData, bufferID, offset, bytesToRead, bufferDataStartPointer);
			bufferDataStartPointer += bytesToRead;
			offset += vertexBufferLayout.GetStride();
		}

		/*const char* bufferPointerBegin = static_cast<const char*>(bufferData);
		const char* bufferPointerEnd = bufferPointerBegin + vertexBufferDataSize;

		while (bufferPointerBegin < bufferPointerEnd)
		{
			glNamedBufferSubData(bufferID, offset, element.GetSizeofCustomType(element.type), bufferPointerBegin);
			bufferPointerBegin += element.GetSizeofCustomType(element.type);
			offset += vertexBufferLayout.GetStride();
		}*/

		//glNamedBufferSubData(bufferID, offset, bufferSize, bufferData);

		//glNamedBufferSubData(
		//vertexBuffer,
		//0,
		//sizeof(glm::vec3) * vertexData.positions.size(),
		//vertexData.positions.data());

		//glNamedBufferSubData(
		//vertexBuffer,
		//sizeof(glm::vec3) * vertexData.positions.size(),
		//sizeof(glm::vec3) * vertexData.colors.size(),
		//vertexData.colors.data());

		//glNamedBufferSubData(
		//vertexBuffer,
		//sizeof(glm::vec3) * vertexData.positions.size() + sizeof(glm::vec3) * vertexData.colors.size(),
		//sizeof(glm::vec2) * vertexData.texCoords.size(),
		//vertexData.texCoords.data());
	}

	/*void VertexBuffer::OverwriteVertexBufferData(unsigned int layoutLocation, )
	{
		
	}*/
}
