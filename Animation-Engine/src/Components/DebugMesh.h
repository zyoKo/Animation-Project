#pragma once

#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Graphics/RenderApi.h"

namespace Animator
{
	class DebugMesh
	{
	public:
		DebugMesh();

		void Bind() const;

		void Unbind() const;

		~DebugMesh() = default;

		void Draw(const std::shared_ptr<Shader>& shader) const;

	private:
		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		void SetupMesh() const;
	};
}
