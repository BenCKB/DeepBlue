#include "Log.h"

std::shared_ptr<spdlog::logger> Log::logger_st;

void Log::init()
{
	try {
		logger_st = spdlog::stdout_color_mt("Logger");
		logger_st->set_pattern("[%H:%M:%S.%e] %^%v%$");
		
	}
	catch (const spdlog::spdlog_ex& ex) {
		std::cout << "Failed to initialize Logger: " << ex.what() << std::endl;
	}
}
