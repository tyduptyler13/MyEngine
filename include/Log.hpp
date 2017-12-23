#ifndef MYENGINE_LOG
#define MYENGINE_LOG

#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> getLogger(const std::string& name);

#endif //MYENGINE_LOG_HPP
