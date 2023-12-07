#pragma once

namespace AnimationEngine::Physics
{
	class Particle;
}

namespace AnimationEngine::Physics
{
	enum class SpringType
	{
		ELASTIC,
		BEND,
		SHEAR
	};

	class Spring
	{
	public:
		Spring(SpringType type, std::weak_ptr<Particle> p1, std::weak_ptr<Particle> p2, float springRestLength);

		void Update();

		float GetSpringCoefficient() const;
		void SetSpringCoefficient(float newSpringCoefficient);

		float GetDampingCoefficient() const;
		void SetDampingCoefficient(float newDampingCoefficient);

		float GetRestingLength() const;
		void SetRestingLength(float newRestingLength);

		const std::tuple<std::weak_ptr<Particle>, std::weak_ptr<Particle>>& GetEndsOfSpring() const;

		decltype(auto) GetEndsOfSpringTest() const
		{
			return (endsOfSpring);
		}

		SpringType GetSpringType() const;

	private:
		SpringType type;

		std::tuple<std::weak_ptr<Particle>, std::weak_ptr<Particle>> endsOfSpring;
		
		// Hook's coefficient
		float springCoefficient;

		float dampingCoefficient;

		float restingLength;

		void SetRestingLength();

		void SatisfyConstraints();
	};
}
