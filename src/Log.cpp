//
// Created by Tyler Scott on 12/22/17.
//

#include <memory>

#include "Log.hpp"

static std::shared_ptr<spdlog::sinks::simple_file_sink_mt> myEngineLog = std::make_shared<spdlog::sinks::simple_file_sink_mt>(
        "MyEngine.log");

std::shared_ptr<spdlog::logger> getLogger(const std::string& name) {
	return std::make_shared<spdlog::logger>(name, myEngineLog);
}
