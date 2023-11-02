#pragma once

#include <vector>
#include <functional>

namespace AnimationEngine::Math
{
	class GaussianQuadrature
	{
	public:
		GaussianQuadrature(int n)
		{
			switch (n)
			{
			case 2:
				weights = { 1.0, 1.0 };
				abscissa = { -0.5773502691896257, 0.5773502691896257 };
				break;

			case 3:
				weights = { 0.8888888888888888, 0.5555555555555556, 0.5555555555555556 };
				abscissa = { -0.7745966692414834, 0.0, 0.7745966692414834 };
				break;

			case 4:
				weights = { 0.6521451548625461, 0.6521451548625461, 0.3478548451374538, 0.3478548451374538 };
				abscissa = { -0.3399810435848563, 0.3399810435848563, -0.8611363115940526, 0.8611363115940526 };
				break;

			case 5:
			default:
				weights = { 0.5688888888888889, 0.4786286704993665, 0.4786286704993665, 0.2369268850561891, 0.2369268850561891 };
				abscissa = {  -0.9061798459386640, -0.5384693101056831, 0.0, 0.5384693101056831, 0.9061798459386640 };
				break;
			}
		}

		double Integrate(const std::function<double(double)>& f, double a, double b)
		{
			double integral = 0.0;
			double halfLength = (b - a) / 2.0;
			double midpoint = (b + a) / 2.0;

			for (size_t i = 0; i < weights.size(); ++i) 
			{
				integral += weights[i] * f(midpoint + halfLength * abscissa[i]);
			}

			return halfLength * integral;
		}

	private:
		std::vector<double> weights;
		std::vector<double> abscissa;
	};
}