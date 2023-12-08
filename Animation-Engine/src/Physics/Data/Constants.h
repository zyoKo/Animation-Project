#pragma once

namespace AnimationEngine::Physics
{
	// Spring Properties
	static constexpr float ELASTIC_COEFFICIENT = 10.0f;

	static constexpr float BEND_COEFFICIENT = 3.0f;

	static constexpr float SHEAR_COEFFICIENT = 5.0f;

	static constexpr float DAMPING_COEFFICIENT = 1.0f;

	// Cloth Shader Details
	static const std::string CLOTH_SHADER_NAME = "ClothShader";

	static const std::string CLOTH_VERTEX_SHADER_PATH = "./assets/shaders/cloth.vert";

	static const std::string CLOTH_FRAGMENT_SHADER_PATH = "./assets/shaders/cloth.frag";

	// Sphere Shader Details
	static const std::string SPHERE_SHADER_NAME = "SphereShader";

	static const std::string SPHERE_VERTEX_SHADER_PATH = "./assets/shaders/sphere.vert";

	static const std::string SPHERE_FRAGMENT_SHADER_PATH = "./assets/shaders/sphere.frag";

	static constexpr std::string_view SPHERE_MODEL_FILE_PATH = "./assets/sphere/sphere.dae";

	// Particle Default Properties
	static constexpr float CLOTH_PARTICLE_MASS = 1.0f;

	static constexpr float CLOTH_DAMPING_COEFFICIENT = 1.0f;

	// Sphere Default Properties
	static constexpr float DEFAULT_SPHERE_RADIUS = 10.0f;
	static constexpr float SPHERE_RADIUS_RELEASE_MODE = 8.0f;
	static constexpr float SPHERE_RADIUS_DEBUG_MODE = 4.0f;

	static constexpr Math::Vec3F DEFAULT_SPHERE_CENTER_LOCATION = { 10.0f, 15.0f, 10.0f };

	static constexpr float DEFAULT_SPHERE_MOVEMENT_SPEED = 3.0f;

	// Cloth Simulation Constants
	static constexpr float DEFAULT_PARTICLE_MASS = 1.0f;

	static constexpr float DEFAULT_SPRING_REST_LENGTH = 0.0f; // computes spring left based on distance

	static constexpr Math::Vec3F EXTERNAL_FORCE = { 0.0f, -5000.0f, 0.0f };

	static constexpr unsigned CLOTH_WIDTH_RELEASE_MODE = 50;
	static constexpr unsigned CLOTH_HEIGHT_RELEASE_MODE = 50;

	static constexpr unsigned CLOTH_WIDTH_DEBUG_MODE = 20;
	static constexpr unsigned CLOTH_HEIGHT_DEBUG_MODE = 20;
}