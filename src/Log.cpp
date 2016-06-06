#include <iostream>
#include <mutex>

#include "Log.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

//Static private variables.
static mutex l;
static DefaultClock c;

static string getTime(){
	return "[" + to_string(c.getElapsedSeconds()) + "]";
}

Log& Log::operator<<(const std::string s) {

	l.lock();

	std::cout << getTime() << "[Log][" << klass << "] " << s << "\n";

	l.unlock();

	return *this;

}

Log& Log::operator<<(std::ostringstream& ss) {

	l.lock();

	std::cout << getTime() << "[Log][" << klass << "] " << ss.str() << "\n";
	ss.clear();
	ss.str(std::string());

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
