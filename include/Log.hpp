#ifndef MYUPLAY_MYENGINE_LOG
#define MYUPLAY_MYENGINE_LOG

#include <sstream>
#include <string>

#include "Clock.hpp"

namespace MyUPlay {

	namespace MyEngine {

		/**
		 * This class is a useful class for synchronizing logging output.
		 * It both standarizes the output and enforces thread safety.
		 *
		 * To create a log, use your class name to create an instance of log.
		 */
		class Log {

		public:

			std::string klass;

			Log(std::string s) : klass(s) {}

			Log& operator<<(const std::string s);

			Log& operator<<(std::ostringstream& ss);

			Log& error(const std::string s);

			Log& warn(const std::string s);

		};

	}

}

#endif

