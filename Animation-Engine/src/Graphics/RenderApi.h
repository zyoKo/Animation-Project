#pragma once

#include <memory>

#include "Core/Logger/Log.h"
#include "Core/Window/IWindow.h"
#include "OpenGL/OpenGLContext.h"

namespace Animator
{
	class RenderApi
	{
	public:
		static void CreateContext(IWindow* window)
		{
			graphicsContext = std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window->GetNativeWindow()));
		}

		static const std::unique_ptr<IContext>& GetContext()
		{
			ANIM_ASSERT(graphicsContext != nullptr, "Trying to fetch Graphics Context before it was created!");

			return graphicsContext;
		}

	private:
		inline static std::unique_ptr<IContext> graphicsContext{nullptr};
	};
	
}
