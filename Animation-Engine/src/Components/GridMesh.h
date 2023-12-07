#pragma once

namespace AnimationEngine
{
	class Shader;
	class ITexture2D;
	class IVertexArray;
	class IIndexBuffer;
	class IVertexBuffer;
}

namespace AnimationEngine
{
	class GridMesh
	{
	public:
		GridMesh();

		GridMesh(const std::vector<Math::Vector3F>& vertices, 
			const std::vector<Math::Vector2F>& textureCoordinates,
			const std::vector<unsigned>& indices);

		~GridMesh() = default;

		void Bind() const;

		void UnBind() const;

		void Update();

		void SetVertices(const std::vector<Math::Vector3F>& vertices);

		void SetTextureCoordinates(const std::vector<Math::Vector2F>& textureCoordinates);

		void SetIndices(const std::vector<unsigned>& indices);

	private:
		std::vector<Math::Vector3F> vertices;
		std::vector<Math::Vector2F> textureCoordinates;

		std::vector<unsigned> indices;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::shared_ptr<IIndexBuffer> indexBuffer;
		std::weak_ptr<Shader> shader;

		std::weak_ptr<ITexture2D> gridTexture;

		bool dirtyFlag;

		void SetupMesh() const;

		void ReSetupMesh() const;

		void SetupShader();

		void Render();

		void SetupGridTexture();
	};
}
