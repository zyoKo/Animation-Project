#pragma once

#include <memory>
#include <string>

#include "Components/Mesh.h"
#include "Components/Camera/Camera.h"
#include "Core/Window/IWindow.h"
#include "Graphics/OpenGL/Textures/ITexture2D.h"
//#include "Graphics/OpenGL/Buffers/Interfaces/IIndexBuffer.h"
//#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
//#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"

namespace Animator
{
	class AssetManager;
}

namespace Animator
{
	class Application
	{
	public:
		Application(const std::string& name = "Animation Window", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application() = default;

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		virtual void Initialize();

		virtual void Run();

		virtual void Render();

		virtual bool Shutdown();

	private:
		void ProcessInput();

		float deltaTime, lastFrame;

		std::shared_ptr<AssetManager> assetManager;

		std::shared_ptr<Shader> shader;

		std::unique_ptr<IWindow> window;

		//std::shared_ptr<Mesh> mesh;

		Camera camera;

		bool running = true;

		std::shared_ptr<ITexture2D> textureDiffuse;

		// Singleton Class
		static Application* instance;
	};
}
