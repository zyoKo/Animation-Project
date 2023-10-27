#pragma once

namespace AnimationEngine::Math
{
	class HermiteSpline
	{
	public:
		HermiteSpline();

		HermiteSpline(const std::vector<Vector3F>& controlPoints);

		const std::vector<Vector3F>& ComputeSpline();

		void OverwriteControlPoints(const std::vector<Vector3F>& controlPoints);

		const std::vector<Vector3F>& GetSpline() const;

	private:
		std::vector<Vector3F> controlPoints;

		std::vector<Vector3F> spline;

		Vector3F Hermite(const Vector3F& p1, const Vector3F& t1, const Vector3F& p2, const Vector3F& t2, float t);
	};
}
