#pragma once

#include <cstdint>
#include <string>

namespace Animator
{
	struct UniversalWindowData
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		UniversalWindowData(const std::string& title = "Animation Window",
					uint32_t width = 1280,
					uint32_t height = 720)
			:	title(title),
				width(width),
				height(height)
		{
		}
	};

	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual bool WindowShouldClose() = 0;

		virtual void Update() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() = 0;

		virtual void* GetNativeWindow() const = 0;

		static IWindow* Create(const UniversalWindowData& windowData = UniversalWindowData());
	};
}
