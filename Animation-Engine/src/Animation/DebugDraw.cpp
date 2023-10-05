#include <AnimationPch.h>

#include "DebugDraw.h"

#include "Graphics/OpenGL/Shader/Shader.h"

namespace Animator
{
	DebugDraw::DebugDraw()
		:	debugColor{ 0.1f, 0.9f, 0.1f },
			debugShader(nullptr)
	{
	}

	DebugDraw::DebugDraw(Shader* shader)
		:	debugColor{ 0.1f, 0.9f, 0.1f },
			debugShader(shader)
	{
	}

	void DebugDraw::Draw(DebugDrawMode drawMode, const glm::mat4& mvp)
	{
		debugShader->Bind();
		
		glDrawArrays(DebugDrawModeToGLEnum(drawMode), 0, static_cast<int>(debugPoints.size()));

		debugShader->UnBind();
	}

	void DebugDraw::OverwritePoints(const std::vector<glm::vec3>& points)
	{
		this->debugPoints = points;
	}

	void DebugDraw::AddPoints(const glm::vec3& point)
	{
		debugPoints.push_back(point);
	}

	const std::vector<glm::vec3>& DebugDraw::GetPoints() const
	{
		return debugPoints;
	}

	void DebugDraw::SetDebugColor(const glm::vec3& color)
	{
		this->debugColor = color;
	}

	void DebugDraw::SetShader(Shader* shader)
	{
		this->debugShader = shader;
	}
}
