#pragma once

namespace AnimationEngine::Physics
{
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
}