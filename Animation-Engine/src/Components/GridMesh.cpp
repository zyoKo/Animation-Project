#include <AnimationPch.h>

#include "GridMesh.h"

#include "Core/Logger/GLDebug.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"

namespace Animator
{
	GridMesh::GridMesh()
		:	gridColor(DEFAULT_COLOR_DATA),
			dirtyFlag(false)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}

	GridMesh::GridMesh(const std::vector<Math::Vector3F>& colorData)
		:	gridColor(colorData),
			dirtyFlag(false)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}

	void GridMesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void GridMesh::UnBind() const
	{
		vertexArrayObject->UnBind();
	}

	void GridMesh::Update(const std::shared_ptr<Shader>& shader, const glm::mat4& projection, const glm::mat4& view)
	{
		if (dirtyFlag)
		{
			ReSetupMesh();

			dirtyFlag = false;
		}

		Bind();

		shader->Bind();

		shader->SetUniformMatrix4F(projection, "projection");
		shader->SetUniformMatrix4F(view, "view");

		GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 6);

		shader->UnBind();

		UnBind();
	}

	void GridMesh::SetColor(const std::vector<Math::Vector3F>& colorData)
	{
		this->gridColor = colorData;

		dirtyFlag = true;
	}

	void GridMesh::SetupMesh() const
	{
		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, gridColor.size(), false);

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * gridColor.size());

		int layoutLocation = -1;
		
		vertexBuffer->OverwriteVertexBufferData(
			++layoutLocation,
			gridColor.data(),
			GetSizeofCustomType(VertexDataType::Vector3F) * gridColor.size());

		vertexArrayObject->SetBufferData();
	}

	void GridMesh::ReSetupMesh() const
	{
		// TODO: Find a better strategy

		SetupMesh();
	}
}
