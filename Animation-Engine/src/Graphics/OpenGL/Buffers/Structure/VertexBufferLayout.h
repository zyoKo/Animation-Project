#pragma once

#include <vector>

namespace Animator
{
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();

		VertexBufferLayout(const VertexBufferLayout& vertexBufferLayout);

		~VertexBufferLayout() = default;

		VertexBufferLayout& operator=(const VertexBufferLayout& vertexBufferLayout);



	private:
		//std::vector<VertexBufferData> bufferDataStructure;

		int dataPerVertex;
	};
}