#pragma once

#include "Graphics/OpenGL/Interfaces/IVertexArray.h"

namespace Animator
{
	class VertexArray : public IVertexArray
	{
	public:
		VertexArray();

		~VertexArray() override;

		void Bind() override;

		void UnBind() override;

	private:
		unsigned int vertexArrayID;
	};
}
