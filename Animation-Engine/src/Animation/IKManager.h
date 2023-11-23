#pragma once

#include "Components/Types/DebugDrawMode.h"
#include "Core/Utilities/Utilites.h"
#include "DataTypes/AssimpNodeData.h"
#include "Graphics/OpenGL/Buffers/Structure/VertexBufferLayout.h"
#include "Repository/AnimationStorage.h"

namespace AnimationEngine
{
	class Animation;

	class IVertexArray;

	class IVertexBuffer;

	class Shader;
}

namespace AnimationEngine
{
	struct RigidBone
	{
		Math::Vec3F startPosition;
		Math::Vec3F endPosition;
		float length;

		RigidBone(const Math::Vec3F& start, const Math::Vec3F& end)
			:	startPosition(start),
				endPosition(end)
		{
			length = (startPosition - endPosition).Length();
		}
	};

	class IKManager
	{
	public:
		IKManager();

		void Initialize();

		void Update();

		void SetTargetPosition(const Math::Vector3F& targetPosition);

		bool IsFabrikRunning() const;

		std::vector<glm::mat4> GetFinalBoneMatrices() const;

		const std::vector<Math::Vector3F>& GetJointsPosition() const { return debugJoints; }

		void ClearJoints() { debugJoints.clear(); }

		std::pair<AssimpNodeData*, AssimpNodeData*> GetBaseAndEndEffector() const
		{
			return { base, endEffector };
		}

	private:
		std::vector<RigidBone> boneChain;

		std::vector<std::pair<std::string, Math::VQS>> localJoints;	// joints in local space
		std::vector<std::pair<std::string, Math::VQS>> joints;	// joints in world space
		std::vector<std::pair<std::string, Math::VQS>> globalToLocalJoints;

		bool isFabrikRunning;

		float currentTime;

		std::vector<Math::Vector3F> jointPositions;
		std::vector<float> boneLengths;
		float totalBoneLength;
		std::vector<Math::Vector3F> fabrikSolvedJoints;
		Math::Vector3F targetPosition;
		float threshold;
		unsigned maxIterations;

		std::vector<glm::mat4> finalBoneMatrices;
		std::vector<Math::Vector3F> debugJoints;

		std::string baseBoneName;
		std::string endEffectorName;

		Animation* currentAnimation;

		AssimpNodeData* base;
		AssimpNodeData* endEffector;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::weak_ptr<Shader> shader;

		bool FindJointWithName(AssimpNodeData* node, const std::string& name, AssimpNodeData*& result);

		void ReadHierarchyToFrom(const AssimpNodeData* to, AssimpNodeData* from);

		// Pre-Solver Step
		bool ComputeGlobalFromLocalVQS(AssimpNodeData* node, std::vector<Math::Vector3F>& iKChain, const Math::VQS& rootVQS, bool& chainStart);

		// Post-Solver Step
		bool ComputeLocalFromGlobalVQS(AssimpNodeData* node, const Math::VQS& parentVQS, bool& chainStart, int& counter);

		void BackwardSolver(const Math::Vector3F& target);

		void ForwardSolver(const Math::Vector3F& baseLocation);

		bool FABRIKSolver(const Math::Vec3F& targetLocation);

		void SetupMesh() const;

		void SetupShader() const;

		void OverwriteDataInVertexBuffer() const;

		void ComputeBoneLengths(const std::vector<Math::Vector3F>& jointPositions);

		void UpdateBonePositions() const;

		void CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS);

		void ExtractParentJointAndChildJoints(const Math::VQS& parent, const Math::VQS& child);
	};
}
