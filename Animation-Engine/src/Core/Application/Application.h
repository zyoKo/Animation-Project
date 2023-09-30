#pragma once

#include <memory>
#include <string>

#include "Core/Window/IWindow.h"
#include "Graphics/OpenGL/ITexture2D.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IIndexBuffer.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexArray.h"
#include "Graphics/OpenGL/Buffers/Interfaces/IVertexBuffer.h"

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
		std::unique_ptr<IWindow> window;

		bool running = true;

		std::shared_ptr<IVertexArray> vertexArrayObject;
		std::shared_ptr<IVertexBuffer> vertexBuffer;
		std::shared_ptr<IIndexBuffer> indexBuffer;

		std::shared_ptr<ITexture2D> texture;

		// Singleton Class
		static Application* instance;
	};
}
