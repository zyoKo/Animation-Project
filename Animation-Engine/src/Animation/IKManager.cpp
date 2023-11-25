#include <AnimationPch.h>

#include "IKManager.h"

#include <ranges>

#include "Animation/Repository/AnimationStorage.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Animation/Animation.h"
#include "Components/Camera/Camera.h"
#include "Components/Camera/Constants/CameraConstants.h"
#include "Core/Logger/GLDebug.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Core/Utilities/Utilites.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	IKManager::IKManager()
		:	canRunIK(true),
			totalBoneLength(0),
			threshold(1.0f),
			maxIterations(10),
			baseBoneName("mixamorig_LeftShoulder"),
			endEffectorName("mixamorig_LeftHand"),
			currentAnimation(nullptr),
			base(nullptr),
			endEffector(nullptr)
	{
		if (const auto* animationStorage = AnimationStorageLocator::GetAnimationStorage())
		{
			currentAnimation = animationStorage->GetAnimationForCurrentlyBoundIndex();
		}

		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		const std::string& vertexFilePath = "./assets/shaders/IKManager.vert";
		const std::string& fragmentFilePath = "./assets/shaders/IKManager.frag";
		shader = AssetManagerLocator::GetAssetManager()->CreateShader("IKManagerShader", vertexFilePath, fragmentFilePath);

		jointPositions.resize(4);

		SetupMesh();
	}

	void IKManager::Initialize()
	{
		FindJointWithName(&currentAnimation->GetRootNode(), baseBoneName, base);
		FindJointWithName(&currentAnimation->GetRootNode(), endEffectorName, endEffector);

		bool chainStart = false;
		jointPositions.clear();
		ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), jointPositions, Math::VQS(), chainStart);
		ComputeBoneLengths(jointPositions);
		ComputeInitialDirectionAndRotation(jointPositions);
	}

	void IKManager::Update()
	{
		//bool chainStart = false;
		//joints.clear();
		//jointPositions.clear();
		//ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), fabrikSolvedJoints, Math::VQS(), chainStart);
		//for (const auto& [jointName, jointVQS]: joints)
		//{
		//	//auto jointMatrix = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(joint.second);
		//
		//	jointPositions.emplace_back(
		//		jointVQS.GetTranslationVector().x,
		//		jointVQS.GetTranslationVector().y,
		//		jointVQS.GetTranslationVector().z
		//	);
		//
		//	//LOG_WARN("Global Joints {0} Position: ({1}, {2}, {3})", jointName,
		//	//	jointVQS.GetTranslationVector().x,
		//	//	jointVQS.GetTranslationVector().y,
		//	//	jointVQS.GetTranslationVector().z);
		//}
		////LOG_WARN("====================================\n");
		//
		//localJoints.clear();
		//ReadHierarchyToFrom(base, endEffector);
		//for (const auto& [jointName, jointVQS] : localJoints)
		//{
		//	LOG_WARN("LocalJoint {0} Position: ({1}, {2}, {3})", jointName,
		//		jointVQS.GetTranslationVector().x,
		//		jointVQS.GetTranslationVector().y,
		//		jointVQS.GetTranslationVector().z);
		//}
		//LOG_WARN("====================================\n");
		//
		//int counter = 0;
		//globalToLocalJoints.clear();
		//auto modelVQS = Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(currentAnimation->GetRootNode().transformation);
		//ComputeLocalFromGlobalVQS(&currentAnimation->GetRootNode(), Math::VQS(), chainStart, counter);
		//for (const auto& [jointName, jointVQS] : globalToLocalJoints)
		//{
		//	LOG_WARN("Local(From Global) Joint {0} Position: ({1}, {2}, {3})", jointName,
		//		jointVQS.GetTranslationVector().x,
		//		jointVQS.GetTranslationVector().y,
		//		jointVQS.GetTranslationVector().z);
		//}
		//LOG_WARN("====================================\n");

		if (canRunIK)
		{
			FABRIKSolver({ -10.0f, 10.0f, 10.0f });

			OverwriteDataInVertexBuffer();
			SetupShader();
		}
	}

	void IKManager::SetTargetPosition(const Math::Vector3F& targetPosition)
	{
		this->targetPosition = targetPosition;
	}

	bool IKManager::GetCanRunIK() const
	{
		return canRunIK;
	}

	std::pair<AssimpNodeData*, AssimpNodeData*> IKManager::GetBaseAndEndEffector() const
	{
		return { base, endEffector };
	}

	void IKManager::CanRunIK(bool canRun)
	{
		this->canRunIK = canRun;
	}

	bool IKManager::FindJointWithName(AssimpNodeData* node, const std::string& name, AssimpNodeData*& result)
	{
		if (node->name == name)
		{
			LOG_WARN("Voila! Found: {0}", node->name);
			result = node;
			return true;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i)
		{
			if (true == FindJointWithName(node->children[i].get(), name, result))
			{
				return true;
			}
		}

		return false;
	}

	void IKManager::ReadHierarchyToFrom(const AssimpNodeData* to, AssimpNodeData* from)
	{
		AssimpNodeData* currentJoint = from;

		while (currentJoint != to)
		{
			const auto end = 
			{
				currentJoint->localVQS.GetTranslationVector().x,
				currentJoint->localVQS.GetTranslationVector().y,
				currentJoint->localVQS.GetTranslationVector().z
			};

			const auto start = 
			{
				currentJoint->parent->localVQS.GetTranslationVector().x,
				currentJoint->parent->localVQS.GetTranslationVector().y,
				currentJoint->parent->localVQS.GetTranslationVector().z
			};

			currentJoint = currentJoint->parent;
		}
	}

	bool IKManager::ComputeGlobalFromLocalVQS(AssimpNodeData* node, std::vector<Math::Vector3F>& iKChain, const Math::VQS& rootVQS, bool& chainStart)
	{
		if (node == nullptr)
		{
			return false;
		}

		if (node == base)
		{
			chainStart = true;
		}

		if (node->parent != nullptr)
		{
			node->globalVQS = node->parent->globalVQS * node->localVQS;
		}
		else
		{
			node->globalVQS = rootVQS * node->localVQS;
		}

		if (chainStart)
		{
			const auto& globalPosition = node->globalVQS.GetTranslationVector();
			iKChain.emplace_back(globalPosition.x, globalPosition.y, globalPosition.z);
		}

		if (node == endEffector)
		{
			chainStart = false;
			return true;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i)
		{
			if (true == ComputeGlobalFromLocalVQS(node->children[i].get(), iKChain, rootVQS, chainStart))
			{
				return true;
			}
		}

		return false;
	}

	bool IKManager::ComputeLocalFromGlobalVQS(AssimpNodeData* node, const Math::VQS& parentVQS)
	{
		if (node == nullptr) 
		{
			return false;
		}

		if (node->parent != nullptr)
		{
			node->localVQS = node->parent->globalVQS.Inverse() * node->globalVQS;
		}
		else
		{
			node->localVQS = parentVQS.Inverse() * node->globalVQS;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i) 
		{
		    if (true == ComputeLocalFromGlobalVQS(node->children[i].get(), parentVQS))
			{
		        return true;
		    }
		}

		return false;
	}

	void IKManager::BackwardSolver(const Math::Vector3F& target)
	{
		const auto size = static_cast<int>(jointPositions.size());
		if (size > 0)
		{
			jointPositions[size - 1] = target;
		}

		for (int i = static_cast<int>(jointPositions.size()) - 2; i >= 0; --i)
		{
			auto direction = (jointPositions[i] - jointPositions[i + 1]).GetNormalize();
			auto offset = direction * boneLengths[i];

			jointPositions[i] = jointPositions[i + 1] + offset;
		}
	}

	void IKManager::ForwardSolver(const Math::Vector3F& baseLocation)
	{
		const auto size = static_cast<int>(jointPositions.size());
		if (size > 0)
		{
			jointPositions[0] = baseLocation;
		}

		for (int i = 1; i < size; ++i)
		{
			auto direction = (jointPositions[i] - jointPositions[i - 1]).GetNormalize();
			auto offset = direction * boneLengths[i - 1];

			jointPositions[i] = jointPositions[i - 1] + offset;
		}
	}

	void IKManager::ApplyRotationFix(const Math::Vector3F& target)
	{
		jointRotations.clear();

		for (unsigned i = 0; i < jointPositions.size() - 1; ++i)
		{
			Math::Vector3F newDirection;

			if (i == jointPositions.size() - 2)
			{
				newDirection = (target - jointPositions[i]).GetNormalize();
			}
			else
			{
				newDirection = (jointPositions[i + 1] - jointPositions[i]).GetNormalize();
			}

			Math::QuatF newRotation = Math::QuatF::FromTo(initialJointDirections[i], newDirection);

			jointRotations.emplace_back(newRotation * initialJointRotations[i]);
		}
	}

	bool IKManager::FABRIKSolver(const Math::Vec3F& targetLocation)
	{
		const auto size = jointPositions.size();
		if (size == 0)
		{
			return false;
		}

		const float thresholdSquare = threshold * threshold;

		bool chainStart = false;
		jointPositions.clear();
		ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), jointPositions, Math::VQS(), chainStart);
		const auto basePosition = jointPositions[0];

		auto target = targetLocation;
		// if target is too far away
		const auto distanceFromShoulder = (targetLocation - jointPositions[0]).Length();
		if (distanceFromShoulder > totalBoneLength)
		{
			const auto direction = (targetLocation - jointPositions[0]).GetNormalize();
			const auto offset = direction * totalBoneLength;
			target = jointPositions[0] + offset;
		}

		SetTargetPosition(target);

		for (unsigned iteration = 0; iteration < maxIterations; ++iteration)
		{
			Math::Vector3F effector = jointPositions.back();

			const auto distanceBetweenEEAndTarget = (target - effector).Length();
			if (distanceBetweenEEAndTarget < thresholdSquare)
			{
				ApplyRotationFix(target);

				UpdateBonePositionAndRotation();

				ComputeLocalFromGlobalVQS(&currentAnimation->GetRootNode(), currentAnimation->GetRootNode().globalVQS);

				return true;
			}

			BackwardSolver(target);
			ForwardSolver(basePosition);
		}

		ComputeLocalFromGlobalVQS(&currentAnimation->GetRootNode(), currentAnimation->GetRootNode().globalVQS);

		const auto effector = Utils::GLMInternalHelper::ConvertGLMVectorToInternal(endEffector->globalVQS.GetTranslationVector());
		if ((target - effector).Length() < thresholdSquare)
		{
			return true;
		}

		return false;
	}

	void IKManager::SetupMesh() const
	{
		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(jointPositions.size()), false);

		vertexBuffer->SetVertexBufferLayout(layout);
	}

	void IKManager::SetupShader() const
	{
		const auto camera = Camera::GetInstance();
		const glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		const glm::mat4 view = camera->GetViewMatrix();

		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			ANIM_ASSERT(shaderPtr != nullptr, "Shader for IK Manager is nullptr.");
		}

		shaderPtr->Bind();
		shaderPtr->SetUniformMatrix4F(projection, "projection");
		shaderPtr->SetUniformMatrix4F(view, "view");

		vertexArrayObject->Bind();
		GraphicsAPI::GetContext()->EnablePointSize(true);
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::Points), 0, static_cast<unsigned>(jointPositions.size()));
		GraphicsAPI::GetContext()->EnablePointSize(false);
		vertexArrayObject->UnBind();

		shaderPtr->UnBind();
	}

	void IKManager::OverwriteDataInVertexBuffer() const
	{
		const auto& layout = vertexBuffer->GetVertexBufferLayout();
		vertexBuffer->SetSize(layout.GetStride() * static_cast<unsigned>(jointPositions.size()));

		constexpr int layoutLocation = 0;

		const auto bufferSize = 
			static_cast<unsigned>(
				GetSizeofCustomType(layout.GetVertexBufferElements()[0].type) * jointPositions.size());

		vertexBuffer->OverwriteVertexBufferData(
			layoutLocation,
			jointPositions.data(),
			bufferSize);

		vertexArrayObject->SetBufferData();
	}

	void IKManager::ComputeInitialDirectionAndRotation(const std::vector<Math::Vector3F>& jointPositions)
	{
		for (unsigned i = 1; i < jointPositions.size(); ++i)
		{
			auto direction = (jointPositions[i] - jointPositions[i - 1]).GetNormalize();
			initialJointDirections.emplace_back(direction);
		}

		auto temp = endEffector;
		while (temp != base)
		{
			initialJointRotations.emplace_back(temp->parent->globalVQS.GetRotation());
			temp = temp->parent;
		}

		std::ranges::reverse(initialJointRotations);
	}

	void IKManager::ComputeBoneLengths(const std::vector<Math::Vector3F>& jointPositions)
	{
		for (unsigned i = 0; i < jointPositions.size() - 1; ++i)
		{
			const auto boneLength = (jointPositions[i] - jointPositions[i + 1]).Length();
			totalBoneLength += boneLength;
			boneLengths.emplace_back(boneLength);
		}
	}

	void IKManager::UpdateBonePositionAndRotation() const
	{
		auto temp = endEffector;
		auto i = jointPositions.size() - 1;
		while (temp != base)
		{
			temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));
			
			// set the rotation for all joints except base
			temp->globalVQS.SetRotation(jointRotations[i - 1]);

			--i;
			temp = temp->parent;
		}

		temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));
	}
}
