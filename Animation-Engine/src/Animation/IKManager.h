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
	class IKManager
	{
	public:
		IKManager();

		void Initialize();

		void Update();

		void SetTargetPosition(const Math::Vector3F& targetPosition);

		bool GetCanRunIK() const;

		std::pair<AssimpNodeData*, AssimpNodeData*> GetBaseAndEndEffector() const;

		void CanRunIK(bool canRun);

	private:
		bool canRunIK;

		Math::Vector3F targetPosition;

		std::vector<Math::Vector3F> jointPositions;
		std::vector<Math::Vector3F> initialJointDirections;
		std::vector<Math::QuatF> initialJointRotations;
		std::vector<Math::QuatF> jointRotations;

		std::vector<float> boneLengths;
		float totalBoneLength;
		float threshold;
		unsigned maxIterations;

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
		bool ComputeLocalFromGlobalVQS(AssimpNodeData* node, const Math::VQS& parentVQS);

		// FABRIK Logic
		void BackwardSolver(const Math::Vector3F& target);
		void ForwardSolver(const Math::Vector3F& baseLocation);
		void ApplyRotationFix(const Math::Vector3F& target);
		bool FABRIKSolver(const Math::Vec3F& targetLocation);

		void SetupMesh() const;
		void SetupShader() const;
		void OverwriteDataInVertexBuffer() const;

		void ComputeInitialDirectionAndRotation(const std::vector<Math::Vector3F>& jointPositions);
		void ComputeBoneLengths(const std::vector<Math::Vector3F>& jointPositions);
		void UpdateBonePositionAndRotation() const;
	};
}
