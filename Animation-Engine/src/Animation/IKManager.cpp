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
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Utilites.h"
#include "Graphics/GraphicsAPI.h"

namespace AnimationEngine
{
	IKManager::IKManager()
		:	isFabrikRunning(true),
			currentTime(0.0f),
			totalBoneLength(0),
			threshold(1.0f),
			maxIterations(10),
			baseBoneName("mixamorig_LeftShoulder"),
			endEffectorName("mixamorig_LeftHand"),
			currentAnimation(nullptr),
			base(nullptr),
			endEffector(nullptr)
	{
		auto* animationStorage = AnimationStorageLocator::GetAnimationStorage();
		if (animationStorage)
		{
			currentAnimation = animationStorage->GetAnimationForCurrentlyBoundIndex();
		}

		finalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; ++i)
		{
			finalBoneMatrices.emplace_back(1.0f);
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
		auto result = FindJointWithName(&currentAnimation->GetRootNode(), baseBoneName, base);
		auto result2 = FindJointWithName(&currentAnimation->GetRootNode(), endEffectorName, endEffector);

		ReadHierarchyToFrom(base, endEffector);

		bool chainStart = false;
		jointPositions.clear();
		ComputeGlobalFromLocalVQS(&currentAnimation->GetRootNode(), jointPositions, Math::VQS(), chainStart);
		ComputeBoneLengths(jointPositions);
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

		isFabrikRunning = FABRIKSolver({ -10.0f, 10.0f, 10.0f });
		//if (isFabrikRunning)
		//{
		//	for (auto& jointPosition : jointPositions)
		//	{
		//		LOG_WARN("Global Joints Position: ({0}, {1}, {2})", jointPosition.x, jointPosition.y, jointPosition.z);
		//	}
		//}

		//if (currentAnimation)
		//{
		//	currentTime += currentAnimation->GetTicksPerSecond() * Time::GetDeltaTime();
		//	currentTime = std::fmod(currentTime, currentAnimation->GetDuration());
		//	CalculateBoneTransformWithVQS(&currentAnimation->GetRootNode(), Math::VQS());
		//}

		OverwriteDataInVertexBuffer();
		SetupShader();
	}

	void IKManager::SetTargetPosition(const Math::Vector3F& targetPosition)
	{
		this->targetPosition = targetPosition;
	}

	bool IKManager::IsFabrikRunning() const
	{
		return isFabrikRunning;
	}

	std::vector<glm::mat4> IKManager::GetFinalBoneMatrices() const
	{
		return finalBoneMatrices;
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

			boneChain.emplace_back(end, start);

			localJoints.emplace_back(currentJoint->name, currentJoint->localVQS);

			currentJoint = currentJoint->parent;
		}

		localJoints.emplace_back(currentJoint->name, currentJoint->localVQS);

		std::ranges::reverse(localJoints);
		std::ranges::reverse(boneChain);
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
			// TODO: Just for visualization Removed Later
			joints.emplace_back(node->name, node->globalVQS);

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

	bool IKManager::ComputeLocalFromGlobalVQS(AssimpNodeData* node, const Math::VQS& parentVQS, bool& chainStart, int& counter)
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
			node->localVQS = node->parent->globalVQS.Inverse() * node->globalVQS;
		}
		else
		{
			node->localVQS = parentVQS.Inverse() * node->globalVQS;
		}

		// Compute the local transformation by dividing the global transformation by the parent's global transformation.
		if (chainStart) 
		{
		    globalToLocalJoints.emplace_back(node->name, node->localVQS);
			
			const auto& localPosition = node->localVQS.GetTranslationVector();
			fabrikSolvedJoints.emplace_back(localPosition.x, localPosition.y, localPosition.z);
			++counter;
		}

		if (node == endEffector) 
		{
		    chainStart = false;
		    return true;
		}

		for (unsigned i = 0; i < node->childrenCount; ++i) 
		{
		    if (true == ComputeLocalFromGlobalVQS(node->children[i].get(), parentVQS, chainStart, counter))
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
				UpdateBonePositions();

				int counter = 0;
				fabrikSolvedJoints.clear();
				ComputeLocalFromGlobalVQS(&currentAnimation->GetRootNode(), currentAnimation->GetRootNode().globalVQS, chainStart, counter);

				return true;
			}

			BackwardSolver(target);
			ForwardSolver(basePosition);
		}

		int counter = 0;
		ComputeLocalFromGlobalVQS(&currentAnimation->GetRootNode(), currentAnimation->GetRootNode().globalVQS, chainStart, counter);

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

	void IKManager::ComputeBoneLengths(const std::vector<Math::Vector3F>& jointPositions)
	{
		for (unsigned i = 0; i < jointPositions.size() - 1; ++i)
		{
			const auto boneLength = (jointPositions[i] - jointPositions[i + 1]).Length();
			totalBoneLength += boneLength;
			boneLengths.emplace_back(boneLength);
		}
	}

	void IKManager::UpdateBonePositions() const
	{
		auto temp = endEffector;
		auto i = jointPositions.size() - 1;
		while (temp != base)
		{
			temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));
			--i;

			temp = temp->parent;
		}

		temp->globalVQS.SetTranslationVector(Utils::GLMInternalHelper::ConvertInternalVectorToGLM(jointPositions[i]));
	}

	void IKManager::CalculateBoneTransformWithVQS(AssimpNodeData* node, Math::VQS parentVQS)
	{
		std::string nodeName = node->name;
		auto localVQS = node->localVQS;

		const auto worldVQS = parentVQS * localVQS;

		if (node->parent)
		{
			ExtractParentJointAndChildJoints(parentVQS, localVQS);
		}

		auto boneInfoMap = currentAnimation->GetBoneIDMap();
		if (boneInfoMap.contains(nodeName))
		{
			const auto index = boneInfoMap[nodeName].id;
			const auto& offset = Utils::GLMInternalHelper::ConvertGLMMatrixToVQS(boneInfoMap[nodeName].offset);

			finalBoneMatrices[index] = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(worldVQS * offset);
		}

		for (unsigned i = 0; i < node->childrenCount; ++i)
		{
			CalculateBoneTransformWithVQS(node->children[i].get(), worldVQS);
		}
	}

	void IKManager::ExtractParentJointAndChildJoints(const Math::VQS& parent, const Math::VQS& child)
	{
		const auto parentJoint = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parent);
		const auto childJoint = Utils::GLMInternalHelper::ConvertVQSToGLMMatrix(parent * child);

		const auto parentJointPosition = Math::Vector3F{ parentJoint[3].x, parentJoint[3].y, parentJoint[3].z };
		const auto childJointPosition = Math::Vector3F{ childJoint[3].x, childJoint[3].y, childJoint[3].z };

		debugJoints.push_back(parentJointPosition);
		debugJoints.push_back(childJointPosition);
	}
}
