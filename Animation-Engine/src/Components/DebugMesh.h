#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Graphics/RenderApi.h"

namespace Animator
{
	class DebugMesh
	{
	public:
		DebugMesh(const std::vector<Math::Vector3F>& jointsPosition);

		void Bind() const;

		void Unbind() const;

		~DebugMesh() = default;

		void Draw(const std::shared_ptr<Shader>& shader) const;

		void OverwriteJointsPosition(const std::vector<Math::Vector3F>& jointsPosition);

	private:
		//std::vector<glm::vec3> jointsPosition;
		std::vector<Math::Vector3F> jointsPosition;

		std::vector<glm::quat> jointsRotation;

		std::vector<glm::vec3> jointsScale;

		std::vector<glm::mat4> jointsTransformation;

		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		void SetupMesh() const;
	};
}
