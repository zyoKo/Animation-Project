#pragma once

#include <string>

namespace AnimationEngine
{
	struct WindowData
	{
		std::string title;
		uint32_t width, height;
		bool vSync;
	};
}
