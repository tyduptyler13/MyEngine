#ifndef MYUPLAY_MYENGINE_CLOCK
#define MYUPLAY_MYENGINE_CLOCK

#include <chrono>

namespace MyUPlay {

	namespace MyEngine {

		template <typename Accuracy = double>
		class Clock {

		private:
			std::chrono::steady_clock::time_point oldTime;
			std::chrono::duration<Accuracy, std::milli> elapsedTime;

			bool running = true;

		public:
			const bool autoStart;
			std::chrono::steady_clock::time_point startTime;

			Clock(bool autoStart = true) : autoStart(autoStart) {
				if (autoStart){
					start();
				}
			}

			void start() {
				startTime = std::chrono::steady_clock::now();
				oldTime = startTime;

			}

			void stop() {
				running = false;
			}

			std::chrono::duration<Accuracy> getElapsedTime() {
				return elapsedTime;
			}

			Accuracy getElapsedSeconds() {
				return elapsedTime.count() / 1000;
			}

			std::chrono::duration<Accuracy> getDelta() {

				std::chrono::duration<Accuracy, std::milli> diff = std::chrono::duration<Accuracy>::zero();

				if (autoStart && !running){
					start();
				}

				if (running){

					std::chrono::steady_clock::time_point newTime = std::chrono::steady_clock::now();

					diff = newTime - oldTime;
					oldTime = newTime;

					elapsedTime += diff;

				}

				return diff;

			}

			static Accuracy durationToSeconds(const std::chrono::duration<Accuracy, std::milli>& duration) {
				return duration.count() / 1000;
			}

		};

	}

}

#endif

