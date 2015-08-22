#ifndef MYUPLAY_MYENGINE_CLOCK
#define MYUPLAY_MYENGINE_CLOCK

#include <chrono>

namespace MyUPlay {

	namespace MyEngine {

		template <typename Accuracy = double>
		class Clock {

			private:
				std::chrono::steady_clock::time_point oldTime;
				std::chrono::duration<Accuracy> elapsedTime;

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
					
				}

				std::chrono::duration<Accuracy> getDelta() {
					
					std::chrono::duration<Accuracy> diff = std::chrono::duration_cast<std::chrono::duration<Accuracy>>(0.0);

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

		};

		typedef Clock<> DefaultClock;

	}

}

#endif

