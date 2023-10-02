#include <AnimationPch.h>

#include "Mesh.h"

#include "Core/Logger/GLDebug.h"
#include "Data/Constants.h"

namespace Animator
{
	Mesh::Mesh()
		:	vertices(DEFAULT_VERTICES_DATA),
			colors(DEFAULT_COLOR_DATA),
			textureCoordinates(DEFAULT_TEXTURE_COORDINATES_DATA)//,
			//indices(DEFAULT_INDICES_DATA)
	{
		vertexArrayObject = RenderApi::CreateVertexArray();
		vertexBuffer = RenderApi::CreateVertexBuffer();
		indexBuffer = RenderApi::CreateIndexBuffer();

		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}

	Mesh::Mesh(Vertices_V3F vertices, Colors_V3F colors, Normal_V3F normals, TexCoordinates_V2F textureCoordinates, Indices_V3UI indices)
		:	vertices(std::move(vertices)),
			colors(std::move(colors)),
			normals(std::move(normals)),
			textureCoordinates(std::move(textureCoordinates))//,
			//indices(std::move(indices))
	{
		vertexArrayObject = RenderApi::CreateVertexArray();
		vertexBuffer = RenderApi::CreateVertexBuffer();
		indexBuffer = RenderApi::CreateIndexBuffer();

		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}

	Mesh::Mesh(Vertices_V3F vertices, Normal_V3F normals, TexCoordinates_V2F textureCoordinates, Tangents_V3F tangents, BiTangents_V3F biTangents, std::vector<unsigned> indices)
		:	vertices(std::move(vertices)),
			normals(std::move(normals)),
			textureCoordinates(std::move(textureCoordinates)),
			tangents(std::move(tangents)),
			biTangents(std::move(biTangents)),
			indices(std::move(indices))
			//indices(std::move(indices))
	{
		vertexArrayObject = RenderApi::CreateVertexArray();
		vertexBuffer = RenderApi::CreateVertexBuffer();
		indexBuffer = RenderApi::CreateIndexBuffer();

		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		SetupMesh();
	}


	void Mesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void Mesh::Unbind() const
	{
		vertexArrayObject->UnBind();
	}

	const Vertices_V3F& Mesh::GetVertices() const
	{
		return vertices;
	}

	void Mesh::SetVertices(Vertices_V3F vertexList) noexcept
	{
		this->vertices = std::move(vertexList);
	}

	const Normal_V3F& Mesh::GetNormals() const
	{
		return normals;
	}

	void Mesh::SetNormals(Normal_V3F normalList) noexcept
	{
		this->normals = std::move(normalList);
	}

	const Colors_V3F& Mesh::GetColors() const
	{
		return colors;
	}

	void Mesh::SetColors(Vertices_V3F colorsList) noexcept
	{
		this->colors = std::move(colorsList);
	}

	const TexCoordinates_V2F& Mesh::GetTextureCoordinates() const
	{
		return textureCoordinates;
	}

	void Mesh::SetTextureCoordinates(TexCoordinates_V2F texCoords) noexcept
	{
		this->textureCoordinates = std::move(texCoords);
	}

	const Tangents_V3F& Mesh::GetTangents() const
	{
		return tangents;
	}


	void Mesh::SetTangents(Tangents_V3F tangentsList)
	{
		this->tangents = std::move(tangentsList);
	}

	const BiTangents_V3F& Mesh::GetBiTangents() const
	{
		return biTangents;
	}

	void Mesh::SetBiTangents(BiTangents_V3F biTangentsList)
	{
		this->biTangents = std::move(biTangentsList);
	}

	//const Indices_V3UI& Mesh::GetIndices() const
	//{
	//	return indices;
	//}
	//
	//void Mesh::SetIndices(Indices_V3UI indexList) noexcept
	//{
	//	this->indices = std::move(indexList);
	//}

	const ITexturesList& Mesh::GetTextures() const
	{
		return textures;
	}
	
	void Mesh::SetTextures(ITexturesList textures)
	{
		this->textures = std::move(textures);
	}

	void Mesh::AddTexture(const std::shared_ptr<ITexture2D>& texture)
	{
		if (texture != nullptr)
			textures.push_back(texture);
	}

	void Mesh::Draw(const std::shared_ptr<Shader>& shader) const
	{
		for (int i = 0; i < static_cast<int>(textures.size()); ++i)
		{
			textures[i]->Bind(i);

			shader->SetUniformInt(i, textures[i]->GetTextureName());
		}

		Bind();
		//GL_CALL(glDrawElements, GL_TRIANGLES, GetSizeofCustomType(VertexDataType::Vector3UI) * indices.size(), GL_UNSIGNED_INT, 0);
		GL_CALL(glDrawElements, GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		Unbind();

		for (const auto& texture : textures)
		{
			texture->Bind(0);
		}
	}

	void Mesh::SetupMesh() const
	{
		//indexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3UI) * static_cast<unsigned>(indices.size()));
		indexBuffer->SetSize(sizeof(unsigned) * static_cast<unsigned>(indices.size()));
		indexBuffer->SetData(indices.data());

		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(vertices.size()), false);

		if (!colors.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(colors.size()), false);
		}

		if (!normals.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(normals.size()), false);
		}

		if (!textureCoordinates.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector2F, static_cast<unsigned>(textureCoordinates.size()), false);
		}

		if (!tangents.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(normals.size()), false);
		}

		if (!biTangents.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(normals.size()), false);
		}

		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(vertices.size()));

		int layoutLocation = 0;
		vertexBuffer->OverwriteVertexBufferData(
			layoutLocation++,
			vertices.data(),
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(vertices.size()));

		if (!colors.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				colors.data(),
				GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(colors.size()));
		}

		if (!normals.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				normals.data(),
				GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(normals.size()));
		}

		if (!textureCoordinates.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				textureCoordinates.data(),
				GetSizeofCustomType(VertexDataType::Vector2F) * static_cast<unsigned>(textureCoordinates.size()));
		}

		if (!tangents.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				tangents.data(),
				GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(tangents.size()));
		}

		if (!biTangents.empty())
		{
			vertexBuffer->OverwriteVertexBufferData(
				layoutLocation++,
				biTangents.data(),
				GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(biTangents.size()));
		}

		vertexArrayObject->SetBufferData();
	}
}
