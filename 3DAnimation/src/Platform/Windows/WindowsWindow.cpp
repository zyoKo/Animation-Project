#include "WindowsWindow.h"

#include <iostream>

#include "glad/glad.h"

namespace Animator
{
	static bool isGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::cout << "Failed to Initialized GLFW!" << std::endl;
		std::cout << "GLFW Error: " << error << " : " << description << "\n";
		__debugbreak();
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

	void WindowsWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
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

		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
		{
			std::cout << "Failed to Initialize GLAD!" << std::endl;
			__debugbreak();
		}

		glfwSetWindowUserPointer(window, &windowData);
		SetVSync(true);

		// TODO: Shit ton of call backs here
		// Keyboard, WindowResize, Mouse, Scroll, Position, etc.
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(window);
	}
}
