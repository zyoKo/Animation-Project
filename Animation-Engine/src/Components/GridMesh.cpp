#include <AnimationPch.h>

#include "GridMesh.h"

#include "Camera/Camera.h"
#include "Camera/Constants/CameraConstants.h"
#include "Core/Logger/GLDebug.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Data/Constants.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	GridMesh::GridMesh()
		:	vertices(GRID_VERTICES),
			textureCoordinates(GRID_TEXTURE_COORDINATES),
			indices(GRID_INDICES),
			dirtyFlag(false)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();
		indexBuffer = GraphicsAPI::CreateIndexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);
		vertexArrayObject->SetIndexBuffer(indexBuffer);

		SetupMesh();

		SetupShader();

		SetupGridTexture();
	}

	GridMesh::GridMesh(const std::vector<Math::Vector3F>& vertices,
		const std::vector<Math::Vector2F>& textureCoordinates,
		const std::vector<unsigned>& indices)
		:	vertices(vertices),
			textureCoordinates(textureCoordinates),
			indices(indices),
			dirtyFlag(false)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();
		indexBuffer = GraphicsAPI::CreateIndexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);
		vertexArrayObject->SetIndexBuffer(indexBuffer);

		SetupMesh();

		SetupShader();

		SetupGridTexture();
	}

	void GridMesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void GridMesh::UnBind() const
	{
		vertexArrayObject->UnBind();
	}

	void GridMesh::Update()
	{
		Render();
	}

	void GridMesh::SetVertices(const std::vector<Math::Vector3F>& vertices)
	{
		this->vertices = vertices;

		//dirtyFlag = true;
	}

	void GridMesh::SetTextureCoordinates(const std::vector<Math::Vector2F>& textureCoordinates)
	{
		this->textureCoordinates = textureCoordinates;
	}

	void GridMesh::SetIndices(const std::vector<unsigned>& indices)
	{
		this->indices = indices;

		//dirtyFlag = true;
	}

	void GridMesh::SetupMesh() const
	{
		indexBuffer->SetSize(sizeof(unsigned) * static_cast<unsigned>(indices.size()));
		indexBuffer->SetData(indices.data());

		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(vertices.size()), false);

		if (!textureCoordinates.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector2F, static_cast<unsigned>(textureCoordinates.size()), false);
		}

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(vertices.size()));

		int layoutLocation = -1;
		
		vertexBuffer->OverwriteVertexBufferData(
			++layoutLocation,
			vertices.data(),
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(vertices.size()));

		if (!textureCoordinates.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				++layoutLocation,
				textureCoordinates.data(),
				GetSizeofCustomType(VertexDataType::Vector2F) * static_cast<unsigned>(textureCoordinates.size()));
		}

		vertexArrayObject->SetBufferData();
	}

	void GridMesh::ReSetupMesh() const
	{
		// TODO: Find a better strategy

		SetupMesh();
	}

	void GridMesh::SetupShader()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();
		const std::string gridVertexShaderFile = "./assets/shaders/inf_grid.vert";
		const std::string gridFragmentShaderFile = "./assets/shaders/inf_grid.frag";
		shader = assetManager->CreateShader("GridShader", gridVertexShaderFile, gridFragmentShaderFile);
	}

	void GridMesh::Render()
	{
		if (dirtyFlag)
		{
			ReSetupMesh();

			dirtyFlag = false;
		}

		const auto shaderPtr = shader.lock();
		const auto gridTexturePtr = gridTexture.lock();
		if (!shaderPtr || !gridTexturePtr)
		{
			return;
		}

		const auto* camera = Camera::GetInstance();
		glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		glm::mat4 view = camera->GetViewMatrix();

		shaderPtr->Bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		shaderPtr->SetUniformMatrix4F(projection, "projection");
		shaderPtr->SetUniformMatrix4F(view, "view");
		shaderPtr->SetUniformMatrix4F(model, "model");

		gridTexturePtr->Bind(0);
		shaderPtr->SetUniformInt(0, gridTexturePtr->GetTextureName());

		// Draw Call
		Bind();
		GL_CALL(glDrawElements, GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
		UnBind();

		gridTexturePtr->UnBind();

		shaderPtr->UnBind();
	}

	void GridMesh::SetupGridTexture()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();
		const std::string gridTextureFile = "./assets/grid.png";
		gridTexture = assetManager->CreateTexture(gridTextureFile);

		const auto gridTexturePtr = gridTexture.lock();
		if (!gridTexturePtr)
		{
			LOG_ERROR("Failed to create grid texture!");
			return;
		}

		gridTexturePtr->SetTextureName("gridTexture");
	}
}
