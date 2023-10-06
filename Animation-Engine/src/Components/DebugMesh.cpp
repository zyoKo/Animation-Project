#include <AnimationPch.h>

#include "DebugMesh.h"

#include "Core/Logger/GLDebug.h"

namespace Animator
{
	DebugMesh::DebugMesh(const std::vector<Math::Vector3F>& jointsPosition)
		:	jointsPosition(jointsPosition)
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

		GL_CALL(glDrawArrays, GL_LINES, 0, static_cast<int>(jointsPosition.size()));

		Unbind();
	}

	void DebugMesh::OverwriteJointsPosition(const std::vector<Math::Vector3F>& jointsPosition)
	{
		this->jointsPosition = jointsPosition;
	}

	void DebugMesh::SetupMesh() const
	{
		VertexBufferLayout layout;

		if (!jointsPosition.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, jointsPosition.size(), false);
		}

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * jointsPosition.size());

		int layoutLocation = -1;
		if (!jointsPosition.empty())
		{
			++layoutLocation;
			const auto bufferSize = static_cast<unsigned>(GetSizeofCustomType(layout.GetVertexBufferElements()[layoutLocation].type) * jointsPosition.size());
			vertexBuffer->OverwriteVertexBufferData(layoutLocation, jointsPosition.data(), bufferSize);
		}

		vertexArrayObject->SetBufferData();
	}
}
