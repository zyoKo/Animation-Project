#pragma once

#include <glad/glad.h>

#include "Core/Logger/Log.h"

namespace Animator
{
	enum class VertexDataType
	{
		UnsignedChar,

		Integer,
		Vector2I,
		Vector3I,
		Vector4I,

		UnsignedInt,
		Vector2UI,
		Vector3UI,
		Vector4UI,

		Float,
		Vector2F,
		Vector3F,
		Vector4F,
		None
	};

	struct VertexBufferElements
	{
		VertexDataType type;

		unsigned int offset = 0;

		unsigned int count;

		unsigned char normalized;
	};

	static unsigned int GetSizeofCustomType(VertexDataType customType)
		{
			switch(customType)
			{
			case VertexDataType::UnsignedChar:
				return sizeof(unsigned char);

			case VertexDataType::Integer:
				return sizeof(int);

			case VertexDataType::Vector2I:
				return sizeof(int) * 2;

			case VertexDataType::Vector3I:
				return sizeof(int) * 3;

			case VertexDataType::Vector4I:
				return sizeof(int) * 4;

			case VertexDataType::UnsignedInt:
				return sizeof(unsigned int);

			case VertexDataType::Vector2UI:
				return sizeof(unsigned int) * 2;

			case VertexDataType::Vector3UI:
				return sizeof(unsigned int) * 3;

			case VertexDataType::Vector4UI:
				return sizeof(unsigned int) * 4;

			case VertexDataType::Float:
				return sizeof(float);

			case VertexDataType::Vector2F:
				return sizeof(float) * 2;

			case VertexDataType::Vector3F:
				return sizeof(float) * 3;

			case VertexDataType::Vector4F:
				return sizeof(float) * 4;

			case VertexDataType::None:
				return 0;
			}

			return 0;
		}

		static int GetNumberOfElementsFromType(VertexDataType type)
		{
			switch(type)
			{
			case VertexDataType::UnsignedChar:
			case VertexDataType::UnsignedInt:
			case VertexDataType::Float:
			case VertexDataType::Integer:
				return 1;

			case VertexDataType::Vector2I:
			case VertexDataType::Vector2F:
			case VertexDataType::Vector2UI:
				return 2;

			case VertexDataType::Vector3I:
			case VertexDataType::Vector3UI:
			case VertexDataType::Vector3F:
				return 3;

			case VertexDataType::Vector4I:
			case VertexDataType::Vector4UI:
			case VertexDataType::Vector4F:
				return  4;

			case VertexDataType::None:
				return 0;
			}

			return 0;
		}

		static int GetOpenGLTypeFromCustomType(VertexDataType type)
		{
			switch(type)
			{
			case VertexDataType::UnsignedChar:
				return GL_UNSIGNED_BYTE;
				
			case VertexDataType::Integer:
			case VertexDataType::Vector2I:
			case VertexDataType::Vector3I:
			case VertexDataType::Vector4I:
				return GL_INT;

			case VertexDataType::UnsignedInt:
			case VertexDataType::Vector2UI:
			case VertexDataType::Vector3UI:
			case VertexDataType::Vector4UI:
				return GL_UNSIGNED_INT;

			case VertexDataType::Float:
			case VertexDataType::Vector2F:
			case VertexDataType::Vector3F:
			case VertexDataType::Vector4F:
				return GL_FLOAT;
				
			case VertexDataType::None:
				return GL_NONE;
			}

			return GL_NONE;
		}
}
