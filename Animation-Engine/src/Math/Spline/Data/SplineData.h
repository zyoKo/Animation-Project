#pragma once

namespace AnimationEngine::Math
{
	struct SplineData
	{
		std::vector<Vector3F> controlPoints;

		std::vector<Vector3F> tangents;

		std::vector<Vector3F> spline;

		std::vector<float> cumulativeArcLengths;
	};
}