#include <AnimationPch.h>

#include "HermiteSpline.h"

#include "Constants/SplineConstants.h"

namespace AnimationEngine::Math
{
	HermiteSpline::HermiteSpline()
		//:	controlPoints(TEST_CP_LINE)
		//:	controlPoints(DEFAULT_CONTROL_POINTS_CIRCLE)
		:	controlPoints(DEFAULT_CONTROL_POINTS)
	{
		CalculateTangentsForControlPoints();

		ComputeCumulativeArcLengths();
	}

	HermiteSpline::HermiteSpline(const std::vector<Vector3F>& controlPoints)
		:	controlPoints(controlPoints)
	{
		CalculateTangentsForControlPoints();

		ComputeCumulativeArcLengths();
	}

	Vector3F HermiteSpline::GetPointOnSpline(int segmentIndex, float u) const
	{
		Vector3F cpOne, cpTwo, tanOne, tanTwo;
		GetCPAndTangentForSegmentIndex(segmentIndex, cpOne, tanOne, cpTwo, tanTwo);

		return CalculateSplinePointAux(cpOne, tanOne, cpTwo, tanTwo, u);
	}

	Vector3F HermiteSpline::CalculateSplinePointAux(
		const Vector3F& cpOne, const Vector3F& tanOne,
		const Vector3F& cpTwo, const Vector3F& tanTwo,
		float u)
	{
		const float uSquare = u * u;
		const float uCube	= u * u * u;

		const float h1 =  2.0f * uCube - 3.0f * uSquare + 1.0f;	//  2u^3 - 3u^2 + 1
		const float h2 =  1.0f * uCube - 2.0f * uSquare + u;	//   u^3 - 2u^2 + u
		const float h3 = -2.0f * uCube + 3.0f * uSquare + 0.0f;	// -2u^3 - 3u^2
		const float h4 =  1.0f * uCube - 1.0f * uSquare + 0.0f;	//   u^3 - u^2

		return h1 * cpOne + h2 * tanOne + h3 * cpTwo + h4 * tanTwo;
	}

	Vector3F HermiteSpline::CalculateTangentPoint(int segmentIndex, float u) const
	{
		Vector3F cpOne, cpTwo, tanOne, tanTwo;
		GetCPAndTangentForSegmentIndex(segmentIndex, cpOne, tanOne, cpTwo, tanTwo);

		return CalculateTangentPointAux(cpOne, tanOne, cpTwo, tanTwo, u);
	}

	Vector3F HermiteSpline::CalculateTangentPointAux(
		const Vector3F& p1, const Vector3F& t1,
		const Vector3F& p2, const Vector3F& t2,
		float u)
	{
		const float tSquared = u * u;

		const float h1 =  6.0f * tSquared - 6.0f * u;
		const float h2 =  3.0f * tSquared - 4.0f * u;
		const float h3 = -6.0f * tSquared + 6.0f * u;
		const float h4 =  3.0f * tSquared - 2.0f * u;

		return h1 * p1 + h2 * t1 + h3 * p2 + h4 * t2;
	}

	void HermiteSpline::CalculateTangentsForControlPoints()
	{
		if (controlPoints.empty())
		{
		    LOG_WARN("Set control points first to calculate tangents");
		    return;
		}

		tangents.resize(controlPoints.size());

		// Compute tangents for the control points
		for (size_t i = 0; i < controlPoints.size(); ++i)
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
		        tangents[i] = 0.5f * (controlPoints[i+1] - controlPoints[i-1]);
		    }
		}
	}

	const std::vector<Vector3F>& HermiteSpline::ComputeSpline()
	{
		for (size_t segmentIndex = 0; segmentIndex < controlPoints.size() - 1; ++segmentIndex)
		{
			for (float u = 0.0f; u <= 1.0f; u += STEP_SIZE)
			{
				spline.push_back(GetPointOnSpline(static_cast<int>(segmentIndex), u));
			}
		}

		return spline;
	}

	void HermiteSpline::OverwriteControlPoints(const std::vector<Vector3F>& controlPoints)
	{
		if (controlPoints.empty())
		{
			LOG_WARN("Call 'Clear' if you wanna clear the currently bound control points");
			return;
		}

		this->controlPoints = controlPoints;

		CalculateTangentsForControlPoints();

		ComputeSpline();

		ComputeCumulativeArcLengths();
	}

	const std::vector<Vector3F>& HermiteSpline::GetControlPoints() const
	{
		return controlPoints;
	}

	const std::vector<Vector3F>& HermiteSpline::GetTangents() const
	{
		return tangents;
	}

	const std::vector<Vector3F>& HermiteSpline::GetSpline() const
	{
		return spline;
	}

	const std::vector<float>& HermiteSpline::GetCumulativeArcLengths() const
	{
		return cumulativeArcLengths;
	}

	float HermiteSpline::GetCumulativeArcLength(float u) const
	{
		float length = 0.0f;

		for (size_t index = 0; index < controlPoints.size() - 1; ++index)
		{
			const auto arcLength = SegmentArcLengthUsingGaussianQuadrature(static_cast<int>(index), u);

		    length += arcLength;
		}

		return length;
	}

	float HermiteSpline::FindUUsingBisect(float nS, int segmentIndex) const
	{
	    // Bisection method for the identified segment
	    float uA = 0.0f;
	    float uB = 1.0f;
	    float uM = (uA + uB) * 0.5f;

		// mid = uM
	    float arcLengthMid = SegmentArcLengthUsingGaussianQuadrature(segmentIndex, uM);

	    const float arcLengthOfPreviousSegments = cumulativeArcLengths[segmentIndex];
		float previousMid = -1.0f;
	    while (
			std::abs(arcLengthOfPreviousSegments + arcLengthMid - nS) > MATH_EPSILON &&
			std::abs(uM - previousMid) > MATH_EPSILON)
	    {
	        if (arcLengthOfPreviousSegments + arcLengthMid < nS)
	        {
	            uA = uM;
	        }
	        else
	        {
	            uB = uM;
	        }

			previousMid = uM;
	        uM = (uA + uB) * 0.5f;
	        arcLengthMid = SegmentArcLengthUsingGaussianQuadrature(segmentIndex, uM);
	    }
	
	    return uM;
	}

	Vector3F HermiteSpline::FindPointOnCurve(float nS) const
	{
	    const int segmentIndex = FindSegmentIndex(nS);
		const float u = FindUUsingBisect(nS, segmentIndex);

		return GetPointOnSpline(segmentIndex, u);
	}

	void HermiteSpline::Clear(bool clearSpline /* = true */)
	{
		if (clearSpline)
		{
			this->spline.clear();
		}

		this->controlPoints.clear();

		this->tangents.clear();
	}

	float HermiteSpline::SegmentArcLengthUsingGaussianQuadrature(int segmentIndex, float u) const
	{
	    // 4-point Gaussian Quadrature
	    static constexpr std::array<float, 4> weights  = { 0.3478548f, 0.6521452f, 0.6521452f, 0.3478548f };
	    static constexpr std::array<float, 4> abscissa = { -0.8611363f, -0.3399810f, 0.3399810f, 0.8611363f };
	
	    float integral = 0.0f;
	    for (int i = 0; i < 4; ++i)
	    {
	        const float t = 0.5f * u * (abscissa[i] + 1.0f);
	        const auto tempArcLength = ComputeArcLength(segmentIndex, t);
	        integral += weights[i] * tempArcLength;
	    }
	    integral *= 0.5f * u;
	
	    return integral;
	}

	float HermiteSpline::ComputeArcLength(int segmentIndex, float u) const
	{
	    // get the two points and tangents for the segment
		Vector3F cpOne, tanOne, cpTwo, tanTwo;
		GetCPAndTangentForSegmentIndex(segmentIndex, cpOne, tanOne, cpTwo, tanTwo);
		
		const Vector3F a =  2.0f * cpOne - 2.0f * cpTwo + 1.0f * tanOne + tanTwo;
		const Vector3F b = -3.0f * cpOne + 3.0f * cpTwo - 2.0f * tanOne - tanTwo;
		const Vector3F c =  tanOne;
		
	    const float A =  9.0f * Vector3F::Dot(a, a);
	    const float B = 12.0f * Vector3F::Dot(a, b);
	    const float C =  6.0f * Vector3F::Dot(a, c) + 4.0f * Vector3F::Dot(b, b);
		const float D =  4.0f * Vector3F::Dot(b, c);
	    const float E =	 1.0f *	Vector3F::Dot(c, c);
		
	    const float uSquare = u * u;
	    const float uCube	= u * u * u;
	    const float uPow4	= u * u * u * u;
		
	    // Compute the integrand for arc length
	    return std::sqrtf(A * uPow4 + B * uCube + C * uSquare + D * u + E);
	}

	int HermiteSpline::FindSegmentIndex(float nS) const
	{
		for (size_t i = 1; i < cumulativeArcLengths.size(); ++i)
		{
		    if (cumulativeArcLengths[i] > nS)
		    {
		        return static_cast<int>(i - 1);
		    }
		}

		return static_cast<int>(cumulativeArcLengths.size()) - 1;
	}

	void HermiteSpline::ComputeCumulativeArcLengths()
	{
		if (controlPoints.empty())
		{
			return;
		}

		cumulativeArcLengths.resize(controlPoints.size());
		cumulativeArcLengths[0] = 0.0f;

		for (int segmentIndex = 1; segmentIndex < static_cast<int>(controlPoints.size()); ++segmentIndex)
		{
			const float segmentLength = SegmentArcLengthUsingGaussianQuadrature(segmentIndex - 1, 1.0f);
			cumulativeArcLengths[segmentIndex] = cumulativeArcLengths[segmentIndex - 1] + segmentLength;
		}
	}

	void HermiteSpline::GetCPAndTangentForSegmentIndex(
		int segmentIndex,
		Vector3F& cpOne, Vector3F& tanOne,
		Vector3F& cpTwo, Vector3F& tanTwo) const
	{
		cpOne = controlPoints[segmentIndex];
		cpTwo = controlPoints[segmentIndex + 1];
		tanOne = tangents[segmentIndex];
		tanTwo = tangents[segmentIndex + 1];
	}
}
