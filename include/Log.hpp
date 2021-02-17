#pragma once

#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> getLogger(const std::string& name);
