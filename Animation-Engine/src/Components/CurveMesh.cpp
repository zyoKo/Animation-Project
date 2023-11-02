#include <AnimationPch.h>

#include "CurveMesh.h"

#include "Camera/Camera.h"
#include "Core/Logger/GLDebug.h"
#include "Core/Utilities/Time.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"
#include "Types/DebugDrawMode.h"

namespace AnimationEngine
{
	CurveMesh::CurveMesh()
		:	spline(std::make_shared<Math::HermiteSpline>())
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		GeneratePath();

		//SetupMesh();
		SetupMeshWithEquallySpacedPoints();
	}

	CurveMesh::CurveMesh(const std::vector<Math::Vector3F>& controlPoints)
		:	spline(std::make_shared<Math::HermiteSpline>())
	{
		vertexArrayObject = GraphicsAPI::CreateVertexArray();
		vertexBuffer = GraphicsAPI::CreateVertexBuffer();

		vertexArrayObject->SetVertexBuffer(vertexBuffer);

		spline->OverwriteControlPoints(controlPoints);

		GeneratePath();

		//SetupMesh();
		SetupMeshWithEquallySpacedPoints();
	}

	void CurveMesh::Bind() const
	{
		vertexArrayObject->Bind();
	}

	void CurveMesh::UnBind() const
	{
		vertexArrayObject->UnBind();
	}

	void CurveMesh::Draw()
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
		GL_CALL(glDrawArrays, DrawModeToGLEnum(DebugDrawMode::LineStrip), 0, static_cast<int>(splinePoints.size() + equallySpacedPoints.size()));
		UnBind();

		shaderPtr->UnBind();
	}

	void CurveMesh::SetShader(const std::shared_ptr<Shader>& shader)
	{
		this->shader = shader;
	}

	const std::vector<Math::Vector3F>& CurveMesh::GetSplinePoints() const
	{
		return splinePoints;
	}

	const std::vector<Math::Vector3F>& CurveMesh::GetEquallySpacedPoints() const
	{
		return equallySpacedPoints;
	}

	std::weak_ptr<Math::HermiteSpline> CurveMesh::GetSpline() const
	{
		return spline;
	}

	void CurveMesh::GeneratePath()
	{
		this->splinePoints = spline->ComputeSpline();

		ComputeEquallySpacedPoints();
	}

	void CurveMesh::ComputeEquallySpacedPoints()
	{
	    const auto totalArcLength = spline->GetCumulativeArcLength(1.0f);
	    constexpr int numSteps = 5000;
	    const float delta = totalArcLength / numSteps;

		auto point = spline->GetControlPoints()[0];
		point.y += 1.0f;
	    equallySpacedPoints.push_back(point);

	    float distanceOnCurve = 0.0f;	// Not normalized
	    for(int i = 1; i < numSteps; ++i)
	    {
	        distanceOnCurve += delta;

	        point = spline->FindPointOnCurve(distanceOnCurve);
	        point.y += 1.0f;	// Just to visualize, delete later

	        equallySpacedPoints.push_back(point);
	    }

		point = spline->GetControlPoints().back();
		point.y += 1.0f;
	    equallySpacedPoints.push_back(point);
	}

	void CurveMesh::SetupMesh() const
	{
		VertexBufferLayout layout;
		if (!splinePoints.empty())
		{
			layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(splinePoints.size()), false);
		}
		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(splinePoints.size()));

		int layoutLocation = -1;
		if (!splinePoints.empty())
		{
			++layoutLocation;
			const auto bufferSize = static_cast<unsigned>(GetSizeofCustomType(layout.GetVertexBufferElements()[layoutLocation].type) * splinePoints.size());
			vertexBuffer->OverwriteVertexBufferData(layoutLocation, splinePoints.data(), bufferSize);
		}

		vertexArrayObject->SetBufferData();
	}

	void CurveMesh::SetupMeshWithEquallySpacedPoints() const
	{
		std::vector<Math::Vector3F> concatenatedSplines(splinePoints.size() + equallySpacedPoints.size());
		for (size_t i = 0; i < splinePoints.size(); ++i)
		{
			concatenatedSplines[i] = splinePoints[i];
		}
		for (size_t i = splinePoints.size(); i < splinePoints.size() + equallySpacedPoints.size(); ++i)
		{
			concatenatedSplines[i] = equallySpacedPoints[i - splinePoints.size()];
		}

		VertexBufferLayout layout;
		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(concatenatedSplines.size()), false);
		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(concatenatedSplines.size()));

		constexpr int layoutLocation = 0;
		const auto bufferSize = static_cast<unsigned>(GetSizeofCustomType(layout.GetVertexBufferElements()[layoutLocation].type) * concatenatedSplines.size());
		vertexBuffer->OverwriteVertexBufferData(layoutLocation, concatenatedSplines.data(), bufferSize);

		vertexArrayObject->SetBufferData();
	}
}
