#pragma once

#include <memory>

#ifdef ANIM_DEBUG
	#define ANIM_ENABLE_ASSERTS
#endif

#ifdef ANIM_ENABLE_ASSERTS
	#define ANIM_ASSERT(condition, ...) {\
		if(!(condition))\
		{\
			LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
			__debugbreak();\
		}\
	}\
	while(0)
#else
	#define ANIM_ASSERT(x, ...) while(0)
#endif
