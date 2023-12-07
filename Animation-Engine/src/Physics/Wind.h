#pragma once

#include "Core/Utilities/Time.h"
#include "Particle/Particle.h"

namespace AnimationEngine::Physics
{
	static constexpr Math::Vec3F DEFAULT_WIND_DIRECTION = { 1.0f, 0.0f, 0.0f };
	static constexpr float DEFAULT_WIND_STRENGTH = 1.0f;

	class Wind
	{
	public:
		Wind(const Math::Vec3F& direction = DEFAULT_WIND_DIRECTION, float strength = DEFAULT_WIND_STRENGTH)
			:	direction(direction),
				strength(strength),
				baseStrength(100.0f),
				oscillationRate(0.5f),
				waveLength(0.5f)
		{ }

		void Update(const std::vector<std::shared_ptr<Particle>>& particles)
		{
			const auto dt = Time::GetDeltaTime();

			strength = baseStrength * (1.0f + std::sin(dt * oscillationRate));

			for (auto& particle : particles)
			{
				if (!particle)
				{
					continue;
				}

				const float waveFactor = std::sin(dt + particle->GetPosition().x * waveLength) * std::cos(dt + particle->GetPosition().z * waveLength);

				const auto totalForce = GetForce() * waveFactor;

				particle->AddForce(totalForce);
			}
		}

		Math::Vec3F GetForce() const
		{
			return direction * strength;
		}

	private:
		Math::Vec3F direction;

		float strength;

		float baseStrength;

		float oscillationRate;

		float waveLength;
	};
}
