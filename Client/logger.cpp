#include "logger.h"

extern bool g_isService;


std::shared_ptr<spdlog::logger> get_logger() {
	if (g_isService)
		return spdlog::basic_logger_mt("logger", get_config("logger", "path"));
	
	return spdlog::stdout_color_mt("console");
}


std::shared_ptr<spdlog::logger> logger = get_logger();
