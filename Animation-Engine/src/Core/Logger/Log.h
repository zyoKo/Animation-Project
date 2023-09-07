#pragma once

#include <memory>

#include "Assertion.h"
#include "spdlog/spdlog.h"

namespace Animator
{
	class Log
	{
	public:
		static void Initialize();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return logger;}

	private:
		static std::shared_ptr<spdlog::logger> logger;
	};
}

#define LOG_TRACE(...)		Animator::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)		Animator::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)		Animator::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		Animator::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	Animator::Log::GetLogger()->critical(__VA_ARGS__)