#include <AnimationPch.h>

#include "Particle.h"

#include "Physics/Integrators/RungeKuttaIntegrator.h"
#include "Math/KinematicEquations/Kinematics.h"

namespace AnimationEngine::Physics
{
	Particle::Particle(const Math::Vec3F& initialPosition, float mass /* = 1.0f */, float damping /* = 1.0f */)
		:	isStatic(false),
			position(initialPosition),
			lastPosition(initialPosition),
			mass(mass),
			damping(damping)
	{
		RungeKuttaIntegrator::SetVelocityEquation(Math::Kinematics::VelocityAsFunctionOfAccelerationAndTime);
	}

	Particle::~Particle()
	{
		RungeKuttaIntegrator::ClearEquation();
	}

	void Particle::Update()
	{
		if (isStatic)
		{
			return;
		}

		RungeKuttaIntegrator::Integrate(
			velocity,
			force / mass,
			[&](const Math::Vec3F& newPosition){ this->AddToCurrentPosition(newPosition); },
			[&](const Math::Vec3F& newVelocity){ this->AddToCurrentVelocity(newVelocity); },
			[&]{ this->ClearForces(); });
	}

	bool Particle::IsStatic() const
	{
		return isStatic;
	}

	void Particle::SetIsStatic(bool value)
	{
		this->isStatic = value;
	}

	void Particle::ToggleIsStatic()
	{
		isStatic = !isStatic;
	}

	const Math::Vec3F& Particle::GetPosition() const
	{
		return position;
	}

	Math::Vec3F& Particle::GetPosition()
	{
		return position;
	}

	void Particle::SetPosition(const Math::Vec3F& newPosition)
	{
		lastPosition = position;
		position = newPosition;
	}

	void Particle::AddToCurrentPosition(const Math::Vec3F& changeInPosition)
	{
		lastPosition = position;
		position += changeInPosition;
	}

	const Math::Vec3F& Particle::GetLastPosition() const
	{
		return lastPosition;
	}

	const Math::Vec2F& Particle::GetTextureCoordinates() const
	{
		return texCoord;
	}

	Math::Vec2F& Particle::GetTextureCoordinates()
	{
		return texCoord;
	}

	void Particle::SetTextureCoordinates(const Math::Vec2F& newTexCoords)
	{
		this->texCoord = newTexCoords;
	}

	float Particle::GetMass() const
	{
		return mass;
	}

	void Particle::SetMass(float newMass)
	{
		this->mass = newMass;
	}

	const Math::Vec3F& Particle::GetVelocity() const
	{
		return velocity;
	}

	Math::Vec3F& Particle::GetVelocity()
	{
		return velocity;
	}

	void Particle::SetVelocity(const Math::Vec3F& newVelocity)
	{
		this->velocity = newVelocity;
	}

	void Particle::AddToCurrentVelocity(const Math::Vec3F& changeInVelocity)
	{
		this->velocity += changeInVelocity;
	}

	void Particle::AddForce(const Math::Vec3F& newForce)
	{
		force += newForce;
	}

	void Particle::ClearForces()
	{
		force.x = 0.0f;
		force.y = 0.0f;
		force.z = 0.0f;
	}
}
