#pragma once
#include "Particle/Particle.h"

namespace AnimationEngine::Physics
{
	class Sphere
	{
	public:
		Sphere(const Math::Vec3F& center, float radius)
			:	center(center), radius(radius)
		{ }

		Math::Vec3F center;

		float radius;
	};

	static void HandleCollisionWithSphere(Sphere& sphere, const std::vector<std::shared_ptr<Particle>>& particles)
	{
		for (auto& particle : particles)
		{
			if (!particle)
			{
				continue;
			}

			Math::Vec3F direction = particle->GetPosition() - sphere.center;
			const float distance = direction.Length();

			// Check if the particle is inside the sphere
			if (distance < sphere.radius) 
			{
			    Math::Vec3F normalizedDirection = direction.GetNormalize();

			    // Set the particle's position to the surface of the sphere
			    Math::Vec3F newPosition = sphere.center + normalizedDirection * sphere.radius;
			    particle->SetPosition(newPosition);
			    
			    // Optional: Adjust the particle's velocity if required
			    // particle->SetVelocity(adjustedVelocity);
			}
		}
	}
}
