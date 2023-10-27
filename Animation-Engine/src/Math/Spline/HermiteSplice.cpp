#include <AnimationPch.h>

#include "HermiteSpline.h"

#include "Constants/SplineConstants.h"

namespace AnimationEngine::Math
{
	HermiteSpline::HermiteSpline()
		:	controlPoints(DEFAULT_CONTROL_POINTS)
	{
	}

	HermiteSpline::HermiteSpline(const std::vector<Vector3F>& controlPoints)
		:	controlPoints(controlPoints)
	{
	}

	Vector3F HermiteSpline::Hermite(const Vector3F& p1, const Vector3F& t1, const Vector3F& p2, const Vector3F& t2, float t)
	{
		const float tSquared = t * t;
		const float tCubed	 = t * t * t;

		const float h1 =  2.0f * tCubed - 3.0f * tSquared + 1.0f;
		const float h2 = tCubed - 2.0f * tSquared + t;
		const float h3 = -2.0f * tCubed + 3.0f * tSquared;
		const float h4 = tCubed - tSquared;

		return h1 * p1 + h2 * t1 + h3 * p2 + h4 * t2;
	}

	const std::vector<Vector3F>& HermiteSpline::ComputeSpline()
	{
		std::vector<Vector3F> tangents(controlPoints.size());

		// Compute perpendicular tangents for the control points
		for (size_t i = 0; i < controlPoints.size() - 1; ++i)
		{
			if (i == 0)
			{
				tangents[i] = controlPoints[1] - controlPoints[0];
			}
			else if (i == controlPoints.size() - 1)
			{
				tangents[i] = controlPoints[i] - controlPoints[i - 1];
			}
			else
			{
				tangents[i] = controlPoints[i+1] - controlPoints[i];
			}
		}

		for (size_t i = 0; i < controlPoints.size() - 1; ++i)
		{
			const Vector3F P1 = controlPoints[i];
			const Vector3F T1 = tangents[i];
			const Vector3F P2 = controlPoints[i+1];
			const Vector3F T2 = tangents[i+1];

			for (float t = 0.0f; t <= 1.0f; t += 0.001f)
			{
				spline.push_back(Hermite(P1, T1, P2, T2, t));
			}
		}

		return spline;
	}

	void HermiteSpline::OverwriteControlPoints(const std::vector<Vector3F>& controlPoints)
	{
		this->controlPoints = controlPoints;

		ComputeSpline();
	}

	const std::vector<Vector3F>& HermiteSpline::GetSpline() const
	{
		return spline;
	}
}