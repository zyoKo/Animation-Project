#pragma once

#include "Math/Spline/HermiteSpline.h"

namespace AnimationEngine
{
	class IVertexArray;
	class IVertexBuffer;
	class Shader;

	class CurveMesh
	{
	public:
		CurveMesh();

		CurveMesh(const std::vector<Math::Vector3F>& controlPoints);

		void Bind() const;

		void UnBind() const;

		void Draw();

		void SetShader(const std::shared_ptr<Shader>& shader);

		const std::vector<Math::Vector3F>& GetSplinePoints() const;
		const std::vector<Math::Vector3F>& GetEquallySpacedPoints() const;
		std::weak_ptr<Math::HermiteSpline> GetSpline() const;

	private:
		std::vector<Math::Vector3F> splinePoints;

		std::vector<Math::Vector3F> equallySpacedPoints;

		std::shared_ptr<Math::HermiteSpline> spline;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::weak_ptr<Shader> shader;

		void SetupMesh() const;

		void SetupMeshWithEquallySpacedPoints() const;

		void GeneratePath();

		void ComputeEquallySpacedPoints();
	};
}
