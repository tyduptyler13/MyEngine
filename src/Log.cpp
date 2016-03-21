#include "Log.hpp"

using namespace MyUPlay::MyEngine;

//Static private variables.
Log Log::instance;
std::mutex Log::lock;
DefaultClock Log::clock;

