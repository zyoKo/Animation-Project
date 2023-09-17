#include "AnimationPch.h"

#include "VertexArray.h"

#include "Core/Logger/GLDebug.h"
#include "glad/glad.h"

namespace Animator
{
	VertexArray::VertexArray()
	{
		GL_CALL(glCreateVertexArrays, 1, &vertexArrayID);
		GL_CALL(glBindVertexArray, vertexArrayID);
	}

	VertexArray::~VertexArray()
	{
		GL_CALL(glDeleteVertexArrays, 1, &vertexArrayID);
	}

	void VertexArray::Bind()
	{
		GL_CALL(glBindVertexArray, vertexArrayID);
	}

	void VertexArray::UnBind()
	{
		GL_CALL(glBindVertexArray, 0);
	}
}
