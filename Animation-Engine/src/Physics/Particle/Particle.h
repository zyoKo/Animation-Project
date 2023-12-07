#pragma once

namespace AnimationEngine::Physics
{
	class Particle
	{
	public:
		Particle() = default;

		Particle(const Math::Vec3F& initialPosition, float mass = 1.0f, float damping = 1.0f);

		~Particle();

		void Update();

		// Particle Static/Non-static
		bool IsStatic() const;
		void SetIsStatic(bool value);
		void ToggleIsStatic();

		// Current Position
		const Math::Vec3F& GetPosition() const;
		Math::Vec3F& GetPosition();
		void SetPosition(const Math::Vec3F& newPosition);
		void AddToCurrentPosition(const Math::Vec3F& changeInPosition);

		// Last Position
		const Math::Vec3F& GetLastPosition() const;

		// Tex-Coordinates
		const Math::Vec2F& GetTextureCoordinates() const;
		Math::Vec2F& GetTextureCoordinates();
		void SetTextureCoordinates(const Math::Vec2F& newTexCoords);

		// Mass
		float GetMass() const;
		void SetMass(float newMass);

		const Math::Vec3F& GetVelocity() const;
		Math::Vec3F& GetVelocity();
		void SetVelocity(const Math::Vec3F& newVelocity);
		void AddToCurrentVelocity(const Math::Vec3F& changeInVelocity);

		// Add force to the current force
		void AddForce(const Math::Vec3F& newForce);

		void ClearForces();

	private:
		bool isStatic;

		Math::Vec3F position;
		Math::Vec3F lastPosition;
		Math::Vec2F texCoord;

		float mass;

		float damping;

		Math::Vec3F velocity;

		Math::Vec3F force;
	};
}