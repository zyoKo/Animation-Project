#pragma once

#include "Math/Math.h"
#include "Math/Spline/HermiteSpline.h"

namespace AnimationEngine
{
	class Shader;
	class IVertexArray;
}

namespace AnimationEngine
{
	class PointMesh
	{
	public:
		PointMesh();

		void SetPosition(const Math::Vector3F& newPosition);
		const Math::Vector3F& GetPosition() const;

		const Math::QuatF& GetRotation() const;

		void SetSpline(const std::weak_ptr<Math::HermiteSpline>& spline);

		void Draw();

		void Reset();

	private:
		Math::Vector3F position;

		Math::QuatF rotation;

		float moveSpeed;

		float totalArcLength;

		float distanceTraveled;

		float timeInSeconds;

		std::vector<Math::Vector3F> equallySpacedPoints;

		std::weak_ptr<Math::HermiteSpline> spline;

		std::shared_ptr<IVertexArray> vertexArrayObject;

		std::shared_ptr<Shader> shader;

		void SetupShader();

		void UpdatePositionOnSpline();
	};
}
