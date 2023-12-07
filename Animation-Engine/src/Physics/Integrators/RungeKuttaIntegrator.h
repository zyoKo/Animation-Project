#pragma once

#include <functional>
#include <queue>

#include "Core/Utilities/Time.h"

namespace AnimationEngine::Physics
{
	class RungeKuttaIntegrator
	{
	public:
		static void SetVelocityEquation(std::function<Math::Vec3F(Math::Vec3F, Math::Vec3F, float)> velocityEquation)
		{
			if (equation.has_value())
			{
				return;
			}

			equation = std::move(velocityEquation);
		}

		static void Integrate(const Math::Vec3F& currentVelocity, const Math::Vec3F& acceleration, 
		                      const std::function<void(Math::Vec3F)>& setNewPosition = nullptr, 
		                      const std::function<void(Math::Vec3F)>& setNewVelocity = nullptr,
		                      const std::function<void()>& clearForce = nullptr)
		{
			if (!equation.has_value())
			{
				LOG_INFO("Equation not set, cannot integrate!");
				return;
			}

			const auto dt = Time::GetDeltaTime();

			static constexpr std::array value { 0.0f, 0.5f , 0.5f, 1.0f };

			std::vector<Math::Vec3F> newVelocities;
			newVelocities.reserve(4);

			for (unsigned i = 0; i < ORDER; ++i)
			{
				newVelocities.emplace_back(equation.value()(currentVelocity, acceleration, value[i] * dt) * dt);
			}

			// 1.0f / 6.0f = 0.1666667f
			const auto newPosition = 
				(0.1666667f) * 
				(newVelocities[0] + 2.0f * newVelocities[1] + 2.0f * newVelocities[2] + newVelocities[3]) * 
				dt;

			const auto newVelocity = acceleration * dt;

			// Should clear force after that
			if (clearForce != nullptr)
			{
				clearForce();
			}

			if (setNewPosition != nullptr)
			{
				setNewPosition(newPosition);
			}

			if (setNewVelocity != nullptr)
			{
				setNewVelocity(newVelocity);
			}
		}

		static void ClearEquation()
		{
			equation.reset();
		}

	private:
		inline static std::optional<std::function<Math::Vec3F(Math::Vec3F, Math::Vec3F, float)>> equation;

		inline static constexpr unsigned ORDER{ 4 };
	};
}
