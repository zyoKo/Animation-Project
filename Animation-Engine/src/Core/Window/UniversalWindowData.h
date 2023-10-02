#pragma once

#include <string>

#include "UniversalWindowConstants.h"

namespace Animator
{
	struct UniversalWindowData
	{
		std::string title;

		uint32_t width;

		uint32_t height;

		UniversalWindowData(const std::string& title = WINDOW_TITLE, uint32_t width = WINDOW_WIDTH, uint32_t height = WINDOW_HEIGHT)
			:	title(title),
				width(width),
				height(height)
		{
		}
	};
}
