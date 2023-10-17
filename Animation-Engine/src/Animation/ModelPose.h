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

		void Update(float animationTime);

		const std::vector<glm::vec3>& GetJointsPosition() const;
		void SetJointsPosition(const std::vector<glm::vec3>& position);

		const std::vector<glm::quat>& GetJointsRotation() const;
		void SetJointsRotation(const std::vector<glm::quat>& rotation);

		const std::vector<glm::vec3>& GetJointsScale() const;
		void SetJointsScale(const std::vector<glm::vec3>& scale);

		const glm::mat4& GetLocalTransform() const;

	private:
		glm::mat4 localTransform;

		std::vector<glm::vec3> jointsPosition;

		std::vector<glm::quat> jointsRotation;

		std::vector<glm::vec3> jointsScale;
	};
}
