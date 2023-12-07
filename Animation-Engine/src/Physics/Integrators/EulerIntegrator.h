#pragma once

namespace AnimationEngine::Physics
{
	template <typename T>
	class EulerIntegrator
	{
	public:
		EulerIntegrator(const std::function<void()>& equation, T initialValue, T stepSize)
			:	equation(equation), y(initialValue), h(stepSize)
		{ }

		void Integrate(T x0, T xEnd)
		{
			T x = x0;
			while (x < xEnd)
			{
				y = y + h * equation(x, y);
				x = x + h;
			}
		}

		T GetResult() const
		{
			return y;
		}

	private:
		std::function<void()> equation;

		T y;
		T h;
	};
}