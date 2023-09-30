#include "AnimationPch.h"

#include <GLFW/glfw3.h>

#include "Application.h"

#include "Core/Logger/GLDebug.h"
#include "Core/Logger/Log.h"
#include "Graphics/RenderApi.h"
#include "Math/Math.h"
#include "stb_image.h"
#include "Graphics/OpenGL/Texture2D.h"

namespace Animator
{
	Application* Application::instance = nullptr;

	struct Mesh
	{
		Math::Vec3F vertices;
		Math::Vec3F colors;
		Math::Vec2F textureCoords;
	};

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		if (!instance)
		{
			Log::Initialize();
		}

		ANIM_ASSERT(!instance, "Application Already exists!");
		instance = this;

		window = std::unique_ptr<IWindow>(IWindow::Create({ name, width, height }));

		// Bind Event Callback here
	}

	void Application::Initialize()
	{
		
	}

	std::string ReadFile(const std::string& filepath)
	{
		std::string shaderSource;

		try
		{
			std::ifstream stream;

			stream.exceptions(std::ios::failbit | std::ios::badbit);
			stream.open(filepath, std::ios::binary);

			stream.seekg(0, std::ios::end);
			auto fileSize = stream.tellg();

			if(fileSize > 0)
			{
				shaderSource.resize(fileSize);
				stream.seekg(0, std::ios::beg);
				stream.read(shaderSource.data(), fileSize);
			}
			else
			{
				LOG_ERROR("Shader File is empty: {0}", filepath);
				__debugbreak();
			}
		}
		catch(std::exception& e)
		{
			LOG_ERROR("Failed to open shader source file: {0}\nException Raised: {1}", filepath, e.what());
			__debugbreak();
		}
		
		return shaderSource;
	}

	void Application::Run()
	{
		const std::string textureFile = "./assets/digipen.jpg";
		int width, height, depth;

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(textureFile.c_str(), &width, &height, &depth, 4);

		ANIM_ASSERT(data != nullptr, "Failed to load texture from file!");

		texture = RenderApi::CreateTexture2D(data, width, height, depth);
		texture->Bind(0);

		stbi_image_free(data);

		std::vector<Math::Vec3F> vertices;
		vertices.push_back({  0.5f,  0.5f, 0.0f });
		vertices.push_back({  0.5f, -0.5f, 0.0f });
		vertices.push_back({ -0.5f, -0.5f, 0.0f });
		vertices.push_back({ -0.5f,  0.5f, 0.0f });

		std::vector<Math::Vec3F> colors;
		colors.push_back({ 1.0f, 0.0f, 0.0f });
		colors.push_back({ 0.0f, 1.0f, 0.0f });
		colors.push_back({ 0.0f, 0.0f, 1.0f });
		colors.push_back({ 1.0f, 1.0f, 0.0f });

		std::vector<Math::Vec2F> textureCoords;
		textureCoords.push_back({ 1.0f, 1.0f });
		textureCoords.push_back({ 1.0f, 0.0f });
		textureCoords.push_back({ 0.0f, 0.0f });
		textureCoords.push_back({ 0.0f, 1.0f });

		std::vector<Math::Vec3UI> indexList;
		indexList.push_back({ 0, 1, 3 });
		indexList.push_back({ 1, 2, 3 });

		vertexArrayObject = RenderApi::CreateVertexArray();

		// Set Index Buffer
		indexBuffer = RenderApi::CreateIndexBuffer();
		indexBuffer->SetSize(GetSizeofCustomType(VertexDataType::Vector3UI) * indexList.size());
		indexBuffer->SetData(indexList.data());

		// Set Vertex Buffer
		vertexBuffer = RenderApi::CreateVertexBuffer();

		VertexBufferLayout layout;
		layout.AddBufferElement(VertexDataType::Vector3F, 4, false);
		layout.AddBufferElement(VertexDataType::Vector3F, 4, false);
		layout.AddBufferElement(VertexDataType::Vector2F, 4, false);
		vertexBuffer->SetVertexBufferLayout(layout);

		vertexBuffer->SetSize(layout.GetStride() * 4);

		//std::vector<std::pair<const void*, unsigned int>> test;
		//test.reserve(3);
		//test.emplace_back(vertices.data(),		VertexBufferElements::GetSizeofCustomType(VertexDataType::Vector3F));
		//test.emplace_back(colors.data(),		VertexBufferElements::GetSizeofCustomType(VertexDataType::Vector3F));
		//test.emplace_back(textureCoords.data(), VertexBufferElements::GetSizeofCustomType(VertexDataType::Vector2F));

		vertexBuffer->OverwriteVertexBufferData(0, vertices.data(), GetSizeofCustomType(VertexDataType::Vector3F) * vertices.size());
		vertexBuffer->OverwriteVertexBufferData(1, colors.data(), GetSizeofCustomType(VertexDataType::Vector3F) * colors.size());
		vertexBuffer->OverwriteVertexBufferData(2, textureCoords.data(), GetSizeofCustomType(VertexDataType::Vector2F) * textureCoords.size());

		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);
		vertexArrayObject->SetBufferData();

		const std::string vertexShaderFile = "./assets/basic.vert";
		const std::string fragmentShaderFile = "./assets/basic.frag";

		const std::string vertexShaderSource = ReadFile(vertexShaderFile);
		const std::string fragmentShaderSource = ReadFile(fragmentShaderFile);

		Shader shader("SimpleShader", vertexShaderSource, fragmentShaderSource);
		shader.Bind();
		shader.SetUniformInt(0, "ourTexture");

		// GAME LOOP ////////////////////////////
		while (running && !window->WindowShouldClose())
		{
			RenderApi::GetContext()->ClearColor();
			RenderApi::GetContext()->ClearBuffer();
			vertexArrayObject->Bind();
			GL_CALL(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// Swap buffer and poll events
			window->Update();
		}
	}

	void Application::Render()
	{
	}

	bool Application::Shutdown()
	{
		running = false;
		return true;
	}
}
