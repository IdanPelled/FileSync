#include "logger.h"


std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("logger", get_config("logger", "path"));
