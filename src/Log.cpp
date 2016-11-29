#include <iostream>
#include <mutex>

#include "Log.hpp"
#include "Clock.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

//Static private variables.
static mutex l;
static Clock<> c;

static string getTime(){
	c.getDelta(); //Tells the clock to update its elapsed time.
	return "[" + to_string(c.getElapsedSeconds()) + "]";
}

Log& Log::log(const std::string s) {

	l.lock();

	std::cout << getTime() << "[Log][" << klass << "] " << s << "\n";

	l.unlock();

	return *this;

}

Log& Log::error(const std::string s) {
	l.lock();

	std::cerr << getTime() << "[ERROR][" << klass << "] " << s << "\n";

	l.unlock();

	return *this;
}

Log& Log::warn(const std::string s) {
	l.lock();

	std::cout << getTime() << "[Warn][" << klass << "] " << s << "\n";

	l.unlock();

	return *this;
}
