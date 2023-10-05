#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"

namespace Animator
{
	class Shader;

	enum class DebugDrawMode
	{
		Points,
		Lines,
		LineStrip,
		LineLoop,
		Triangles,
		TriangleStrip,
		TriangleFan
	};

	static int DebugDrawModeToGLEnum(DebugDrawMode drawMode)
	{
		switch(drawMode)
		{
		case DebugDrawMode::Points:
			return GL_POINTS;

		case DebugDrawMode::Lines:
			return GL_LINES;

		case DebugDrawMode::LineStrip:
			return GL_LINE_STRIP;

		case DebugDrawMode::LineLoop:
			return GL_LINE_LOOP;

		case DebugDrawMode::Triangles:
			return GL_TRIANGLES;

		case DebugDrawMode::TriangleStrip:
			return GL_TRIANGLE_STRIP;

		case DebugDrawMode::TriangleFan:
			return GL_TRIANGLE_FAN;
		}

		return -1;
	}

	class DebugDraw
	{
	public:
		DebugDraw();

		DebugDraw(Shader* shader);

		void Draw(DebugDrawMode drawMode, const glm::mat4& mvp);

		void OverwritePoints(const std::vector<glm::vec3>& points);

		void AddPoints(const glm::vec3& point);

		const std::vector<glm::vec3>& GetPoints() const;

		void SetDebugColor(const glm::vec3& color);

		void SetShader(Shader* shader);

	private:
		std::vector<glm::vec3> debugPoints;

		glm::vec3 debugColor;

		Shader* debugShader;
	};
}
