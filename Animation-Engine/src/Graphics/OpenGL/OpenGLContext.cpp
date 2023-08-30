#include "OpenGLContext.h"

#include "Core/Logger/Log.h"
#include "Core/Logger/GLDebug.h"

namespace Animator
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:	window(window)
	{
		glfwMakeContextCurrent(window);

		const int gladLoadStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ANIM_ASSERT(gladLoadStatus, "Failed to load GLAD!");

		const GLubyte* glInfo = GL_CALL(glGetString, GL_VENDOR);
		LOG_INFO("OpenGL Vendor: {0}", reinterpret_cast<const char*>(glInfo));

		glInfo = GL_CALL(glGetString, GL_RENDERER);
		LOG_INFO("OpenGL Renderer Info: {0}", reinterpret_cast<const char*>(glInfo));

		glInfo = GL_CALL(glGetString, GL_VERSION);
		LOG_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glInfo));

		glInfo = GL_CALL(glGetString, GL_EXTENSIONS);
		std::string extensionsStr(reinterpret_cast<const char*>(glInfo));
		size_t pos = 0;
		while ((pos = extensionsStr.find(' ', pos)) != std::string::npos) {
			extensionsStr.replace(pos, 1, "\n");
			pos += 1; // Move past the replaced newline
		}
		LOG_INFO("OpenGL Supported Extension: {0}", extensionsStr);
	}

	void OpenGLContext::AddContext()
	{
		glfwMakeContextCurrent(window);
	}

	void OpenGLContext::ClearContext()
	{
		glfwMakeContextCurrent(nullptr);
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(window);
	}

	void OpenGLContext::ClearBuffer()
	{
		GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::ClearColor()
	{
		GL_CALL(glClearColor, 0.2f, 0.3f, 0.3f, 1.0f);
	}
}
