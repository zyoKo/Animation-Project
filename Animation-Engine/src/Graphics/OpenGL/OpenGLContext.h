#pragma once

#include "GLFW/glfw3.h"
#include "Graphics/Interfaces/IContext.h"

namespace Animator
{
	class OpenGLContext : public IContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void AddContext() override;

		void ClearContext() override;

		void SwapBuffer() override;

		void ClearBuffer() override;

		void ClearColor() override;

	private:
		GLFWwindow* window;
	};
}
