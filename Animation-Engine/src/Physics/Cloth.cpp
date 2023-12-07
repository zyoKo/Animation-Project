#include <AnimationPch.h>

#include "Cloth.h"

#include "Components/Camera/Camera.h"
#include "Components/Camera/Constants/CameraConstants.h"
#include "Core/Logger/GLDebug.h"
#include "Core/ServiceLocators/Assets/AssetManagerLocator.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/OpenGL/Shader/Shader.h"
#include "Particle/Particle.h"
#include "Spring/Spring.h"

namespace AnimationEngine::Physics
{
	Cloth::Cloth(unsigned width, unsigned height, float particleMass, float sprintRestLength)
		:	width(width),
			height(height)
	{
		particleVAO = GraphicsAPI::CreateVertexArray();
		particleVB = GraphicsAPI::CreateVertexBuffer();
		particleVAO->SetVertexBuffer(particleVB);

		elasticVAO = GraphicsAPI::CreateVertexArray();
		elasticVB = GraphicsAPI::CreateVertexBuffer();
		elasticVAO->SetVertexBuffer(elasticVB);

		bendVAO = GraphicsAPI::CreateVertexArray();
		bendVB = GraphicsAPI::CreateVertexBuffer();
		bendVAO->SetVertexBuffer(bendVB);

		shearVAO = GraphicsAPI::CreateVertexArray();
		shearVB = GraphicsAPI::CreateVertexBuffer();
		shearVAO->SetVertexBuffer(shearVB);

		InitializeParticles(particleMass);

		InitializeSprings(sprintRestLength);

		SetupMesh();

		SetupShader();
	}

	void Cloth::Update()
	{
		UpdateParticles();
		
		UpdateSprings();

		UpdateVertexBuffers();

		RenderParticles(DebugDrawMode::Points);

		RenderElasticSprings(DebugDrawMode::Lines);

		RenderBendSprings(DebugDrawMode::Lines);

		RenderShearSprings(DebugDrawMode::Lines);
	}

	void Cloth::ResetSimulation()
	{
		//Clear();

		//InitializeParticles();
	}

	void Cloth::Clear()
	{
		particleList.clear();
		springs.clear();
	}

	std::vector<std::weak_ptr<Particle>> Cloth::GetWeakParticles() const
	{
		std::vector<std::weak_ptr<Particle>> weakParticles;
		weakParticles.reserve(particleList.size());

		for (const auto& particle : weakParticles)
		{
			weakParticles.emplace_back(particle);
		}

		return weakParticles;
	}

	const std::vector<std::shared_ptr<Particle>>& Cloth::GetParticles() const
	{
		return particleList;
	}

	void Cloth::InitializeParticles(float particleMass)
	{
		constexpr float particleSpacing = 0.5f;

		if (!particleList.empty())
		{
			particleList.clear();
		}

		for (unsigned y = 0; y < height; ++y)
		{
			for (unsigned x = 0; x < width; ++x)
			{
				constexpr float yOffset = 30.0f; // Offset on the Y-axis

				Math::Vec3F position(static_cast<float>(x) * particleSpacing, yOffset, static_cast<float>(y) * particleSpacing);
				auto newParticle = std::make_shared<Particle>(position, particleMass);

				if (x == 0 && y == 0 ||
					x == width - 1 && y == 0 ||
					//x == 0 && y == height - 1 ||
					x == width - 1 && y == height - 1)
				{
					//newParticle->SetIsStatic(true);
				}

				particleList.emplace_back(std::move(newParticle));
			}
		}

		LOG_WARN("Particles in the system: {0}", particleList.size());

		// optimization when resetting simulation
		if (!particleLocations.empty())
		{
			particleLocations.clear();
		}

		particleLocations.resize(particleList.size());
		for (unsigned i = 0; i < particleList.size(); ++i)
		{
			particleLocations[i] = particleList[i]->GetPosition();
		}
	}

	void Cloth::InitializeSprings(float springRestLength)
	{
		size_t elasticSpringCount = 0;
		size_t bendSpringCount = 0;
		size_t shearSpringCount = 0;

		for (unsigned y = 0; y < height; ++y) 
		{
			for (unsigned x = 0; x < width; ++x) 
			{
				// Index Of Current Particle = y * width + x;
				auto currentParticle = particleList[y * width + x];

				// Connect to immediate neighbors = Elastic
				if (x < width - 1) 
				{ // Right
					auto rightParticle = particleList[y * width + x + 1]; // currentIndex + 1
					springs.emplace_back(std::make_shared<Spring>(SpringType::ELASTIC, currentParticle, rightParticle, springRestLength));
					++elasticSpringCount;
				}
				if (y < height - 1) 
				{ // Below
					auto belowParticle = particleList[(y + 1) * width + x]; // currentIndex + width
					springs.emplace_back(std::make_shared<Spring>(SpringType::ELASTIC, currentParticle, belowParticle, springRestLength));
					++elasticSpringCount;
				}

				// Connect to far neighbors = Bend
				if (x < width - 2) 
				{ // Right
					auto farRightParticle = particleList[y * width + x + 2];	// currentIndex + 2
					springs.emplace_back(std::make_shared<Spring>(SpringType::BEND, currentParticle, farRightParticle, springRestLength));
					++bendSpringCount;
				}
				if (y < height - 2) 
				{ // Below
					auto belowParticle = particleList[(y + 2) * width + x];  // currentIndex + width * 2
					springs.emplace_back(std::make_shared<Spring>(SpringType::BEND, currentParticle, belowParticle, springRestLength));
					++bendSpringCount;
				}

				// Connect to diagonal neighbors = Shear
				if (x < width - 1 && y < height - 1) 
				{ // Bottom Right
					auto diagonalLeftParticle = particleList[(y + 1) * width + x + 1];	// currentIndex + width + 1
					springs.emplace_back(std::make_shared<Spring>(SpringType::SHEAR, currentParticle, diagonalLeftParticle, springRestLength));
					++shearSpringCount;
				}
				if (x > 0 && y < height - 1) 
				{ // Bottom Left
					auto diagonalRightParticle = particleList[(y + 1) * width + x - 1]; // currentIndex + width - 1
					springs.emplace_back(std::make_shared<Spring>(SpringType::SHEAR, currentParticle, diagonalRightParticle, springRestLength));
					++shearSpringCount;
				}
			}
		}

		LOG_INFO("Elastic Spring in the system: {0}", elasticSpringCount);
		LOG_INFO("Bend Spring in the system: {0}", bendSpringCount);
		LOG_INFO("Shear Spring in the system: {0}", shearSpringCount);

		elasticSprings.resize(elasticSpringCount * 2);
		bendSprings.resize(bendSpringCount * 2);
		shearSprings.resize(shearSpringCount * 2);
	}

	void Cloth::SetupMesh() const
	{
		VertexBufferLayout layout;

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(particleLocations.size()), false);
		particleVB->SetVertexBufferLayout(layout);
		particleVB->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(particleLocations.size()));

		layout.Clear();

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(elasticSprings.size()), false);
		elasticVB->SetVertexBufferLayout(layout);
		elasticVB->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(elasticSprings.size()));

		layout.Clear();

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(bendSprings.size()), false);
		bendVB->SetVertexBufferLayout(layout);
		bendVB->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(bendSprings.size()));

		layout.Clear();

		layout.AddBufferElement(VertexDataType::Vector3F, static_cast<unsigned>(shearSprings.size()), false);
		shearVB->SetVertexBufferLayout(layout);
		shearVB->SetSize(GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(shearSprings.size()));
	}

	void Cloth::SetupShader()
	{
		auto* assetManager = AssetManagerLocator::GetAssetManager();
		const std::string vertexShaderPath = "./assets/shaders/cloth.vert";
		const std::string fragmentShaderPath = "./assets/shaders/cloth.frag";
		shader = assetManager->CreateShader("ClothShader", vertexShaderPath, fragmentShaderPath);
	}

	void Cloth::UpdateVertexBuffers() const
	{
		particleVB->OverwriteVertexBufferData(
			0,
			particleLocations.data(),
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(particleLocations.size()));

		particleVAO->SetBufferData();
		
		elasticVB->OverwriteVertexBufferData(
			0, 
			elasticSprings.data(), 
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(elasticSprings.size()));
		
		elasticVAO->SetBufferData();

		bendVB->OverwriteVertexBufferData(
			0, 
			bendSprings.data(), 
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(bendSprings.size()));
		
		bendVAO->SetBufferData();

		shearVB->OverwriteVertexBufferData(
			0, 
			shearSprings.data(), 
			GetSizeofCustomType(VertexDataType::Vector3F) * static_cast<unsigned>(shearSprings.size()));
		
		shearVAO->SetBufferData();
	}

	void Cloth::RenderParticles(DebugDrawMode mode) const
	{
		// Draw cloth points
		const auto* camera = Camera::GetInstance();
		const auto viewMatrix = camera->GetViewMatrix();
		const auto projectionMatrix = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);

		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			return;
		}

		particleVAO->Bind();

		shaderPtr->Bind();
		shaderPtr->SetUniformMatrix4F(viewMatrix, "view");
		shaderPtr->SetUniformMatrix4F(projectionMatrix, "projection");

		GraphicsAPI::GetContext()->EnablePointSize(true);
		GL_CALL(glDrawArrays, DrawModeToGLEnum(mode), 0, static_cast<unsigned>(particleLocations.size()));
		GraphicsAPI::GetContext()->EnablePointSize(false);

		shaderPtr->UnBind();

		particleVAO->UnBind();
	}

	void Cloth::RenderElasticSprings(DebugDrawMode mode) const
	{
		const auto* camera = Camera::GetInstance();
		const auto viewMatrix = camera->GetViewMatrix();
		const auto projectionMatrix = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		
		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			return;
		}
		
		elasticVAO->Bind();
		
		shaderPtr->Bind();
		shaderPtr->SetUniformMatrix4F(viewMatrix, "view");
		shaderPtr->SetUniformMatrix4F(projectionMatrix, "projection");
		
		GraphicsAPI::GetContext()->EnablePointSize(true);
		GL_CALL(glDrawArrays, DrawModeToGLEnum(mode), 0, static_cast<unsigned>(elasticSprings.size()));
		GraphicsAPI::GetContext()->EnablePointSize(false);
		
		shaderPtr->UnBind();
		
		elasticVAO->UnBind();
	}

	void Cloth::RenderBendSprings(DebugDrawMode mode) const
	{
		const auto* camera = Camera::GetInstance();
		const auto viewMatrix = camera->GetViewMatrix();
		const auto projectionMatrix = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		
		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			return;
		}
		
		bendVAO->Bind();
		
		shaderPtr->Bind();
		shaderPtr->SetUniformMatrix4F(viewMatrix, "view");
		shaderPtr->SetUniformMatrix4F(projectionMatrix, "projection");
		
		GraphicsAPI::GetContext()->EnablePointSize(true);
		GL_CALL(glDrawArrays, DrawModeToGLEnum(mode), 0, static_cast<unsigned>(bendSprings.size()));
		GraphicsAPI::GetContext()->EnablePointSize(false);
		
		shaderPtr->UnBind();
		
		bendVAO->UnBind();
	}

	void Cloth::RenderShearSprings(DebugDrawMode mode) const
	{
		const auto* camera = Camera::GetInstance();
		const auto viewMatrix = camera->GetViewMatrix();
		const auto projectionMatrix = glm::perspective(glm::radians(camera->GetZoom()), 1280.0f / 720.0f, CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);
		
		const auto shaderPtr = shader.lock();
		if (!shaderPtr)
		{
			return;
		}
		
		shearVAO->Bind();
		
		shaderPtr->Bind();
		shaderPtr->SetUniformMatrix4F(viewMatrix, "view");
		shaderPtr->SetUniformMatrix4F(projectionMatrix, "projection");
		
		GraphicsAPI::GetContext()->EnablePointSize(true);
		GL_CALL(glDrawArrays, DrawModeToGLEnum(mode), 0, static_cast<unsigned>(shearSprings.size()));
		GraphicsAPI::GetContext()->EnablePointSize(false);
		
		shaderPtr->UnBind();
		
		shearVAO->UnBind();
	}

	void Cloth::UpdateParticles()
	{
		for (unsigned i = 0; i < particleList.size(); ++i)
		{
			particleList[i]->Update();

			particleLocations[i] = particleList[i]->GetPosition();
		}
	}

	void Cloth::UpdateSprings()
	{
		unsigned elasticSpringCount = 0;
		unsigned bendSpringCount = 0;
		unsigned shearSpringCount = 0;

		constexpr unsigned MAX_ITERATIONS = 3;

		for (auto iteration = 0u; iteration < MAX_ITERATIONS; ++iteration)
		{
			for (const auto& spring : springs)
			{
				spring->Update();
			}
		}

		for (const auto& spring : springs)
		{
			auto& [particle1, particle2] = spring->GetEndsOfSpringTest();
			
			switch (const auto type = spring->GetSpringType())
			{
			case SpringType::ELASTIC:
				elasticSprings[elasticSpringCount] = particle1.lock()->GetPosition();
				elasticSprings[elasticSpringCount + 1] = particle2.lock()->GetPosition();
				elasticSpringCount += 2;
				break;
			
			case SpringType::BEND:
				bendSprings[bendSpringCount] = particle1.lock()->GetPosition();
				bendSprings[bendSpringCount + 1] = particle2.lock()->GetPosition();
				bendSpringCount += 2;
				break;
			
			case SpringType::SHEAR:
				shearSprings[shearSpringCount] = particle1.lock()->GetPosition();
				shearSprings[shearSpringCount + 1] = particle2.lock()->GetPosition();
				shearSpringCount += 2;
				break;
			}
		}
	}
}
