#ifndef MYUPLAY_MYENGINE_LOG
#define MYUPLAY_MYENGINE_LOG

#include <mutex>
#include <iostream>
#include <sstream>
#include <string>

#include "Clock.hpp"

namespace MyUPlay {

	namespace MyEngine {

		/**
		 * This class is a useful class for synchronizing logging output.
		 * It both standarizes the output and enforces thread safety.
		 */
		class Log {

		private:

			static Log instance; //Created before main.
			static std::mutex lock;
			static DefaultClock clock;

			Log(){}

		public:

			static Log& getInstance() {
				return Log::instance;
			}

			static std::string getTime(){
				return "[" + std::to_string(clock.getElapsedSeconds()) + "] ";
			}

			friend Log& operator<<(Log& log, const std::string s){

				lock.lock();

				std::cout << getTime() << s << "\n";

				lock.unlock();

				return log;

			}

			friend Log& operator<<(Log& log, std::ostringstream& ss){

				lock.lock();

				std::cout << getTime() << ss.str() << "\n";
				ss.clear();
				ss.str(std::string());

				lock.unlock();

				return log;

			}

		};

	}

}

#endif

