#include "logger.h"

extern bool g_isService;


/**
 * Returns a shared pointer to a logger object.
 *
 * This function returns a shared pointer to a logger object depending
 * on whether the program is running as a service or not.
 *
 * If the program is running as a service, the function returns a logger object that writes
 * to a file specified in the program configuration. If the program is not running as a service,
 * the function returns a logger object that writes to the console.
 *
 * @return A shared pointer to a logger object.
 */
std::shared_ptr<spdlog::logger> get_logger() {
	if (g_isService)
		return spdlog::basic_logger_mt("logger", get_config("logger", "path"));
	
	return spdlog::stdout_color_mt("console");
}


std::shared_ptr<spdlog::logger> logger = get_logger();
