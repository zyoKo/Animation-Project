#include <AnimationPch.h>

#include "Spring.h"

#include "Physics/Data/Constants.h"
#include "Physics/Particle/Particle.h"

namespace AnimationEngine::Physics
{
	Spring::Spring(SpringType type, std::weak_ptr<Particle> p1, std::weak_ptr<Particle> p2, float springRestLength)
		:	type(type),
			endsOfSpring({ std::move(p1), std::move(p2) }),
			dampingCoefficient(0.5f),
			restingLength(springRestLength)
	{
		if (springRestLength == 0.0f)
		{
			SetRestingLength();
		}

		switch(type)
		{
		case SpringType::ELASTIC:
			springCoefficient = ELASTIC_COEFFICIENT;
			break;

		case SpringType::BEND:
			springCoefficient = BEND_COEFFICIENT;
			break;

		case SpringType::SHEAR:
			springCoefficient = SHEAR_COEFFICIENT;
			break;
		}
	}

	void Spring::Update()
	{
		const auto& [p1, p2] = endsOfSpring;

		const auto particle1 = p1.lock();
		const auto particle2 = p2.lock();

		if (!particle1 || !particle2)
		{
			return;
		}

		const float newLength = (particle1->GetPosition() - particle2->GetPosition()).Length();

		const auto dampingForceForParticle1 = -1.0f * dampingCoefficient * particle1->GetVelocity();
		const auto dampingForceForParticle2 = -1.0f * dampingCoefficient * particle2->GetVelocity();

		// No spring force
		if (std::abs(newLength) <= Math::EPSILON)
		{
			particle1->AddForce(dampingForceForParticle1);
			particle2->AddForce(dampingForceForParticle2);

			return;
		}

		float changeInLength = newLength - restingLength;

		changeInLength = changeInLength > 0 ?
			std::min(changeInLength, 1.1f * restingLength - restingLength) :
			std::max(changeInLength, 0.9f * restingLength - restingLength);

		const auto forceDirection = Math::Vec3F::Normalize(particle1->GetPosition() - particle2->GetPosition());
		const auto springForce = springCoefficient * forceDirection * changeInLength;

		particle1->AddForce(dampingForceForParticle1 - springForce);
		particle2->AddForce(dampingForceForParticle2 - springForce);

		// Satisfy Spring Constraints
		SatisfyConstraints();
	}

	float Spring::GetSpringCoefficient() const
	{
		return springCoefficient;
	}

	void Spring::SetSpringCoefficient(float newSpringCoefficient)
	{
		this->springCoefficient = newSpringCoefficient;
	}

	float Spring::GetDampingCoefficient() const
	{
		return dampingCoefficient;
	}

	void Spring::SetDampingCoefficient(float newDampingCoefficient)
	{
		this->dampingCoefficient = newDampingCoefficient;
	}

	float Spring::GetRestingLength() const
	{
		return restingLength;
	}

	void Spring::SetRestingLength(float newRestingLength)
	{
		this->restingLength = newRestingLength;
	}

	const std::tuple<std::weak_ptr<Particle>, std::weak_ptr<Particle>>& Spring::GetEndsOfSpring() const
	{
		return endsOfSpring;
	}

	SpringType Spring::GetSpringType() const
	{
		return type;
	}

	void Spring::SetRestingLength()
	{
		auto& [p1, p2] = endsOfSpring;

		const auto particle1 = p1.lock();
		const auto particle2 = p2.lock();

		ANIM_ASSERT(particle1 != nullptr, "Particle 1 is nullptr!");
		ANIM_ASSERT(particle2 != nullptr, "Particle 2 is nullptr!");

		const auto vector = particle1->GetPosition() - particle2->GetPosition();
		restingLength = Math::Vec3F::Length(vector);
	}

	void Spring::SatisfyConstraints()
	{
		auto& [particle1, particle2] = endsOfSpring;

		const auto p1 = particle1.lock();
		const auto p2 = particle2.lock();

		ANIM_ASSERT(p1 != nullptr, "Particle 1 is nullptr!");
		ANIM_ASSERT(p2 != nullptr, "Particle 2 is nullptr!");

		if (p1->IsStatic() && p2->IsStatic())
		{
			return;
		}

		auto deltaPosition = p2->GetPosition() - p1->GetPosition();
		const float stretchedLength = deltaPosition.Length();

		// Calculate the correction required to satisfy the constraint
		auto correctionVector = deltaPosition / stretchedLength * (stretchedLength - restingLength);

		// Apply half of the correction to each particle, unless one is pinned
		const auto halfCorrection = correctionVector * 0.5f;
		if (!p1->IsStatic() && !p2->IsStatic())
		{
			p1->SetPosition(p1->GetPosition() + halfCorrection);
			p2->SetPosition(p2->GetPosition() - halfCorrection);
		}
		else if (!p1->IsStatic())
		{
			p1->SetPosition(p1->GetPosition() + correctionVector);
		}
		else if (!p2->IsStatic())
		{
			p2->SetPosition(p2->GetPosition() - correctionVector);
		}
	}
}
