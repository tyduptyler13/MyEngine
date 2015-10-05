#ifndef MYUPLAY_MYENGINE_THREADPOOL
#define MYUPLAY_MYENGINE_THREADPOOL

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <atomic>

#include "Log.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class ThreadPool {

			public:
				typedef std::function<void()> Task;

			private:

				std::vector<std::thread> pool;
				std::queue<Task> tasks;

				std::mutex taskLock;

				std::atomic<bool> running;

				Task& getTask(){

					taskLock.lock();

					Task& t = tasks.front();
					tasks.pop();

					taskLock.unlock();

					return t;

				}

				void threadLoop(){

					while(running.load()){

						Task& t = getTask();

						try {
							t();
						} catch (...) {
							Log::getInstance() << "Uncaught error in task!";
						}

					}

				}

			public:

				ThreadPool(unsigned size = std::thread::hardware_concurrency() * 1.5) {
					running.store(true);
					pool.reserve(size);
					for (unsigned i = 0; i < size; ++i){
						pool.push_back(std::thread([this]{threadLoop();}));
					}
				}

				void stop(){
					running.store(false);
				}

				void addTask(Task t){
					
					taskLock.lock();
					tasks.push(t);
					taskLock.unlock();

				}


		};

	}

}

#endif

