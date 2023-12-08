#pragma once
#include "Data/Constants.h"
#include "Particle/Particle.h"

namespace AnimationEngine
{
	class Shader;
	class Model;
}

namespace AnimationEngine::Physics
{
	class Sphere
	{
	public:
		Sphere(const Math::Vec3F& center = DEFAULT_SPHERE_CENTER_LOCATION, float radius = DEFAULT_SPHERE_RADIUS);

		void Initialize(std::string_view filePath = SPHERE_MODEL_FILE_PATH);

		void Render() const;

		const Math::Vec3F& GetCenter() const;
		Math::Vec3F& GetCenter();

		float& GetRadius();

	private:
		std::shared_ptr<Model> sphereModel;

		std::weak_ptr<Shader> shader;

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

			Math::Vec3F direction = particle->GetPosition() - sphere.GetCenter();
			const float distance = direction.Length();

			// Check if the particle is inside the sphere
			if (distance < sphere.GetRadius()) 
			{
			    Math::Vec3F normalizedDirection = direction.GetNormalize();

			    // Set the particle's position to the surface of the sphere
			    Math::Vec3F newPosition = sphere.GetCenter() + normalizedDirection * sphere.GetRadius();
			    particle->SetPosition(newPosition);
			}
		}
	}
}
