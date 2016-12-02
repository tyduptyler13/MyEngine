#ifndef MYUPLAY_MYENGINE_LOG
#define MYUPLAY_MYENGINE_LOG

#include <string>

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

			enum Level {
				DebugLevel = 0,
				LogLevel = 1,
				WarnLevel = 2,
				ErrorLevel = 3
			} level = LogLevel;

			std::string klass;

			Log() = delete; //Must have a name
			Log(std::string s) : klass(s) {}

			Log& debug(const std::string s);

			Log& log(const std::string s);

			Log& error(const std::string s);

			Log& warn(const std::string s);

		};

	}

}

#endif

