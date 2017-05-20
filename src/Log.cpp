#include "Log.hpp"

using namespace std;

class {
	//Register the default logger.
	auto console = spdlog::stdout_logger_mt("MyEngine");
};

