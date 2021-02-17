#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace MyEngine::Log {
	std::unique_ptr<spdlog::logger> getLogger(const std::string& name);
}
