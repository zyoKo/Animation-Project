#pragma once

#include <GLFW/glfw3.h>

#include "Interfaces/IContext.h"

namespace AnimationEngine
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

		void EnableDepthTest(bool value) override;

	private:
		GLFWwindow* window;
	};
}
