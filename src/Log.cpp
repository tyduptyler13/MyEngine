//
// Created by Tyler Scott on 12/22/17.
//

#include "Log.hpp"

static auto myEngineLog = std::make_shared<spdlog::sinks::simple_file_sink_mt>(
        "MyEngine.log");

std::unique_ptr<spdlog::logger> MyEngine::Log::getLogger(const std::string& name) {
	return std::make_unique<spdlog::logger>(name, myEngineLog);
}
