#pragma once

#include "Core/Window/IWindow.h"

#include <GLFW/glfw3.h>

#include "Structures/WindowData.h"

namespace Animator
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const UniversalWindowData& winData);
		~WindowsWindow() override;

		bool WindowShouldClose() override;

		void Update() override;

		uint32_t GetWidth() override;
		uint32_t GetHeight() override;

		void SetVSync(bool enabled) override;
		bool IsVSync() override;

		void* GetNativeWindow() const override;

	private:
		GLFWwindow* window;

		WindowData windowData;

		virtual void Init(const UniversalWindowData& winData);
		virtual void Shutdown();
	};
}
