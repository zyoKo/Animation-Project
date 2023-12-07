#pragma once

#include "Components/Types/DebugDrawMode.h"

namespace AnimationEngine::Physics
{
	class Spring;
	class Particle;
}

namespace AnimationEngine
{
	class IVertexArray;
	class IVertexBuffer;
	class Shader;
}

namespace AnimationEngine::Physics
{
	class Cloth
	{
	public:
		Cloth(unsigned width, unsigned height, float particleMass, float sprintRestLength = 0.0f);

		void Update();

		void ResetSimulation();

		void Clear();

		std::vector<std::weak_ptr<Particle>> GetWeakParticles() const;
		const std::vector<std::shared_ptr<Particle>>& GetParticles() const;

	private:
		std::vector<std::shared_ptr<Particle>> particleList;
		std::vector<std::shared_ptr<Spring>> springs;
		unsigned width, height;

		std::vector<Math::Vector3F> particleLocations;
		std::vector<Math::Vector3F> elasticSprings;
		std::vector<Math::Vector3F> bendSprings;
		std::vector<Math::Vector3F> shearSprings;

		std::weak_ptr<Shader> shader;

		std::shared_ptr<IVertexArray> particleVAO;
		std::shared_ptr<IVertexBuffer> particleVB;

		std::shared_ptr<IVertexArray> elasticVAO;
		std::shared_ptr<IVertexBuffer> elasticVB;

		std::shared_ptr<IVertexArray> bendVAO;
		std::shared_ptr<IVertexBuffer> bendVB;

		std::shared_ptr<IVertexArray> shearVAO;
		std::shared_ptr<IVertexBuffer> shearVB;

		void InitializeParticles(float particleMass);
		void InitializeSprings(float springRestLength);

		void SetupMesh() const;
		void SetupShader();
		void UpdateVertexBuffers() const;
		void RenderParticles(DebugDrawMode mode) const;
		void RenderElasticSprings(DebugDrawMode mode) const;
		void RenderBendSprings(DebugDrawMode mode) const;
		void RenderShearSprings(DebugDrawMode mode) const;

		void UpdateParticles();
		void UpdateSprings();
	};
}
