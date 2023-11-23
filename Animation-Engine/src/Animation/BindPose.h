#pragma once

#include <ranges>

#include "Animation.h"
#include "Core/Logger/GLDebug.h"
#include "Core/ServiceLocators/Assets/AnimationStorageLocator.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Repository/AnimationStorage.h"

namespace AnimationEngine
{
	class BindPose
	{
	public:
		BindPose()
			:	dirtyFlag(true)
		{
			vertexArrayObject = GraphicsAPI::CreateVertexArray();
			vertexBuffer = GraphicsAPI::CreateVertexBuffer();

			vertexArrayObject->SetVertexBuffer(vertexBuffer);
		}

		void Update()
		{
			const auto* animationStorage = AnimationStorageLocator::GetAnimationStorage();

			const auto& bindPose = animationStorage->GetAnimationForCurrentlyBoundIndex()->GetBindPose();

			for (const auto& bindPoseMatrix : bindPose | std::views::values)
			{
				jointPositions.emplace_back(GetLocationOfJointsFromBindPose(bindPoseMatrix));
			}
		}

		void Draw()
		{
			if (IsDirty())
			{
				auto* assetManager = AssetManagerLocator::GetAssetManager();
				const std::string bindPoseVertexShader = "./assets/shaders/bind_pose.vert";
				const std::string bindPoseFragmentShader = "./assets/shaders/bind_pose.frag";
				shader = assetManager->CreateShader("BindPoseShader", bindPoseVertexShader, bindPoseFragmentShader);

				SetupMesh();

				ClearDirtyFlag();
			}

			const auto* camera = Camera::GetInstance();
			const glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
			const glm::mat4 view = camera->GetViewMatrix();
			auto model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.01f));

			auto shaderPtr = shader.lock();
			if (!shaderPtr)
			{
				LOG_ERROR("Failed to fetch shader!");
				return;
			}

			shaderPtr->Bind();

			shaderPtr->SetUniformMatrix4F(projection, "projection");
			shaderPtr->SetUniformMatrix4F(view, "view");
			shaderPtr->SetUniformMatrix4F(model, "model");

			vertexArrayObject->Bind();
			GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::Lines), 0, static_cast<int>(jointPositions.size()));
			vertexArrayObject->UnBind();

			shaderPtr->UnBind();
		}

		void SetJointPosition(const std::vector<Math::Vector3F>& joints)
		{
			jointPositions = joints;

			SetDirtyFlag();
		}

	private:
		std::vector<Math::Vector3F> jointPositions;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::weak_ptr<Shader> shader;

		bool dirtyFlag;

		void SetDirtyFlag()
		{
			dirtyFlag = true;
		}

		bool IsDirty() const
		{
			return dirtyFlag;
		}

		void ClearDirtyFlag()
		{
			dirtyFlag = false;
		}

		static Math::Vector3F GetLocationOfJointsFromBindPose(const glm::mat4& matrix)
		{
			return { matrix[3].x, matrix[3].y, matrix[3].z };
		}

		void SetupMesh() const
		{
			VertexBufferLayout layout;

			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(jointPositions.size()), false);

			vertexBuffer->SetVertexBufferLayout(layout);

			vertexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(jointPositions.size()));

			int layoutLocation = 0;
			vertexBuffer->OverwriteVertexBufferData(layoutLocation, jointPositions.data(), GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(jointPositions.size()));
		}
	};
}
