#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/fwd.hpp>

namespace Animator
{
	class ModelPose
	{
	public:
		ModelPose();

		~ModelPose() = default;

	private:
		glm::mat3 jointTransform;

		std::vector<glm::vec3> jointPosition;

		std::vector<glm::quat> jointRotation;

		std::vector<glm::vec3> scale;
	};
}
