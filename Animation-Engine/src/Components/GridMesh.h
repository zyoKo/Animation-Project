#pragma once

#include <glm/fwd.hpp>

#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"

namespace Animator
{
	class Shader;
	class ITexture2D;
	class IVertexArray;
	class IIndexBuffer;

	class GridMesh
	{
	public:
		GridMesh();

		GridMesh(const std::vector<Math::Vector3F>& colorData);

		~GridMesh() = default;

		void Bind() const;

		void UnBind() const;

		void Update(const std::shared_ptr<Shader>& shader, const glm::mat4& projection, const glm::mat4& view);

		void SetColor(const std::vector<Math::Vector3F>& colorData);

	private:
		std::vector<Math::Vector3F> gridColor;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;

		bool dirtyFlag;

		void SetupMesh() const;

		void ReSetupMesh() const;
	};
}
