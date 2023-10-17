#pragma once

#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Animator
{
	struct AssimpNodeData
	{
		std::string name;

		glm::mat4 transformation;

		int childrenCount;

		std::vector<AssimpNodeData> children;

		AssimpNodeData()
			:	transformation(glm::mat4(1.0f)),
				childrenCount(0)
		{
		}
	};
}
