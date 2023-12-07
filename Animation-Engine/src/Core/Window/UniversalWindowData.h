#pragma once

#include "UniversalWindowConstants.h"

namespace AnimationEngine
{
	struct UniversalWindowData
	{
		std::string_view title;

		uint32_t width;

		uint32_t height;

		UniversalWindowData(std::string_view title = WINDOW_TITLE, uint32_t width = WINDOW_WIDTH, uint32_t height = WINDOW_HEIGHT)
			:	title(title),
				width(width),
				height(height)
		{
		}
	};
}
