#pragma once

#include <string>

namespace Animator
{
	struct WindowData
	{
		std::string title;
		uint32_t width, height;
		bool vSync;
	};
}
