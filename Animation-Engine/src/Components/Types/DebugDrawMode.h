#pragma once

#include "glad/glad.h"

namespace AnimationEngine
{
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

	static int DrawModeToGLEnum(DebugDrawMode drawMode)
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
}
