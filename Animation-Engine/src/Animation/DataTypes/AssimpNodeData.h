#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace AnimationEngine
{
	struct AssimpNodeData
	{
		std::string name;

		glm::mat4 transformation;

		unsigned childrenCount;

		std::vector<AssimpNodeData> children;

		AssimpNodeData()
			:	transformation(glm::mat4(1.0f)),
				childrenCount(0)
		{
		}
	};
}
