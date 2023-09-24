#pragma once

#include <glad/glad.h>

#include "Core/Logger/Log.h"

namespace Animator
{
	struct VertexBufferElements
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeofType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:
				return 4;

			case GL_UNSIGNED_INT:
				return 4;

			case GL_UNSIGNED_BYTE:	
				return 1;
			}

			ANIM_ASSERT(false, "Type not supported!");

			return 0;
		}
	};
}
