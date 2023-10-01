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

#define LOG_TRACE(message, ...)		Animator::Log::GetLogger()->trace(message, __VA_ARGS__)
#define LOG_INFO(message, ...)		Animator::Log::GetLogger()->info(message, __VA_ARGS__)
#define LOG_WARN(message, ...)		Animator::Log::GetLogger()->warn(message, __VA_ARGS__)
#define LOG_ERROR(message, ...)		Animator::Log::GetLogger()->error(message, __VA_ARGS__)
#define LOG_CRITICAL(message, ...)	Animator::Log::GetLogger()->critical(message, __VA_ARGS__)

#ifdef ANIM_DEBUG
#define ANIM_ENABLE_ASSERTS
#endif

#ifdef ANIM_ENABLE_ASSERTS
#define ANIM_ASSERT(condition, message, ...) \
	do {\
		if(!(condition))\
		{\
			std::string formattedMessage = std::format("Condition \"{0}\" failed! ", #condition); \
			formattedMessage += std::format(message, __VA_ARGS__); \
			LOG_ERROR(formattedMessage);\
			__debugbreak();\
		}\
	}\
	while(0)
#else
#define ANIM_ASSERT(x, ...)
#endif