#pragma once
#include <iostream>
#include <memory>

#include "spdlog\spdlog.h"
#include "spdlog\sinks\stdout_color_sinks.h"

class Log {
public: 
	static void init();

	static std::shared_ptr<spdlog::logger>& GetLogger() { return logger_st; };
private:
	static std::shared_ptr<spdlog::logger> logger_st;
};

#define LOG_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Log::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)
