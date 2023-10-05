#include <AnimationPch.h>

#include "DebugMesh.h"

namespace Animator
{
	DebugMesh::DebugMesh()
	{
		vertexArrayObject = RenderApi::CreateVertexArray();
		vertexBuffer = RenderApi::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}

	void DebugMesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void DebugMesh::Unbind() const
	{
		vertexArrayObject->UnBind();
	}

	void DebugMesh::Draw(const std::shared_ptr<Shader>& shader) const
	{
		Bind();



		Unbind();
	}

	void DebugMesh::SetupMesh() const
	{
		VertexBufferLayout layout;

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride());

		vertexArrayObject->SetBufferData();
	}
}
