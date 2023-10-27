#include <AnimationPch.h>

#include "CurveMesh.h"

#include "Camera/Camera.h"
#include "Core/Logger/GLDebug.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Types/DebugDrawMode.h"

namespace AnimationEngine
{
	CurveMesh::CurveMesh()
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		GeneratePath();

		SetupMesh();
	}

	CurveMesh::CurveMesh(const std::vector<Math::Vector3F>& controlPoints)
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		spline.OverwriteControlPoints(controlPoints);

		GeneratePath();

		SetupMesh();
	}

	void CurveMesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void CurveMesh::UnBind() const
	{
		vertexArrayObject->UnBind();
	}

	void CurveMesh::Draw() const
	{
		const auto shaderPtr = shader.lock();
		if (shaderPtr == nullptr)
		{
			LOG_WARN("ShaderPtr for CurverMesh is nullptr! Cannot draw CurveMesh!");
			return;
		}
		shaderPtr->Bind();

		const auto camera = Camera::GetInstance();
		const glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		const glm::mat4 view = camera->GetViewMatrix();
		shaderPtr->SetUniformMatrix4F(projection, "projection");
		shaderPtr->SetUniformMatrix4F(view, "view");

		Bind();
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::Points), 0, static_cast<int>(splinePoints.size()));
		UnBind();

		shaderPtr->UnBind();
	}

	void CurveMesh::SetShader(const std::shared_ptr<Shader>& shader)
	{
		this->shader = shader;
	}

	void CurveMesh::GeneratePath()
	{
		this->splinePoints = spline.ComputeSpline();
	}

	void CurveMesh::SetupMesh() const
	{
		VertexBufferLayout layout;

		if (!splinePoints.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(splinePoints.size()), false);
		}

		vertexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(splinePoints.size()));

		vertexBuffer->SetVertexBufferLayout(layout);

		int layoutLocation = -1;
		if (!splinePoints.empty())
		{
			++layoutLocation;
			const auto bufferSize = static_cast<unsigned>(GetSizeofCustomType(layout.GetVertexBufferElements()[layoutLocation].type) * splinePoints.size());
			vertexBuffer->OverwriteVertexBufferData(layoutLocation, splinePoints.data(), bufferSize);
		}

		vertexArrayObject->SetBufferData();
	}
}
