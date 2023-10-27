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

		void Draw() const;

		void SetShader(const std::shared_ptr<Shader>& shader);

	private:
		std::vector<Math::Vector3F> splinePoints;

		Math::HermiteSpline spline;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::weak_ptr<Shader> shader;

		void SetupMesh() const;

		void GeneratePath();
	};
}
