#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace Animator
{
	class Log
	{
	public:
		static void Initialize();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; }

	private:
		static std::shared_ptr<spdlog::logger> logger;
	};
}

#define LOG_TRACE(...)		Animator::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)		Animator::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)		Animator::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		Animator::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	Animator::Log::GetLogger()->critical(__VA_ARGS__)

#ifdef ANIM_DEBUG
#define ANIM_ENABLE_ASSERTS
#endif

#ifdef ANIM_ENABLE_ASSERTS
#define ANIM_ASSERT(condition, ...) \
	do {\
		if(!(condition))\
		{\
			LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
			__debugbreak();\
		}\
	}\
	while(0)
#else
#define ANIM_ASSERT(x, ...)
#endif