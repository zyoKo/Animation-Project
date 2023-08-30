#include <AnimationPch.h>

#include "WindowsWindow.h"
#include "Core/Logger/Log.h"
#include "glad/glad.h"
#include "Graphics/RenderApi.h"

namespace Animator
{
	static bool isGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ANIM_ASSERT(false, "Failed to Initialize GLFW: Error: {0} : {1}", error, description);
	}

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	IWindow* IWindow::Create(const UniversalWindowData& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const UniversalWindowData& winData)
	{
		Init(winData);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	bool WindowsWindow::WindowShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void WindowsWindow::Update()
	{
		glfwPollEvents();

		RenderApi::GetContext()->SwapBuffer();
	}

	uint32_t WindowsWindow::GetWidth()
	{
		return windowData.width;
	}

	uint32_t WindowsWindow::GetHeight()
	{
		return windowData.height;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		windowData.vSync = enabled;
	}

	bool WindowsWindow::IsVSync()
	{
		return windowData.vSync;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return window;
	}

	void WindowsWindow::Init(const UniversalWindowData& winData)
	{
		windowData.title = winData.title;
		windowData.width = winData.width;
		windowData.height = winData.height;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (!isGLFWInitialized)
		{
			const int success = glfwInit();
			if (success != GLFW_TRUE)
			{
				glfwSetErrorCallback(GLFWErrorCallback);
			}

			isGLFWInitialized = true;
		}

		window = glfwCreateWindow((int)winData.width, (int)winData.height, winData.title.c_str(), nullptr, nullptr);

		RenderApi::CreateContext(this);

		SetVSync(true);

		// TODO: Shit ton of call backs here
		// Keyboard, WindowResize, Mouse, Scroll, Position, etc.
		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		glfwSetWindowUserPointer(window, &windowData);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}
