
#ifndef EVENT_ENGINE_HPP
#define EVENT_ENGINE_HPP

#include <queue>
#include <vector>
#include <thread>
#include <mutex>

namespace MyEngine {

class Task{

private:

	virtual ~Task();

	std::mutex lock;
	bool canceled;

public:

	/**
	 * Called by the threads. Checks for canceled state.
	 */
	void start();

	inline bool isCanceled(){
		return canceled;
	}

	//Called by start.
	virtual void run() = 0;

	void cancel();

};

/**
 * Special type of task that allows for reentry into
 * the event queue.
 */
class ReusableTask : public Task {

private:

	/**
	 * This is the engine that owns the task.
	 *
	 * This allows the task to be re registered.
	 */
	const TaskEngine engine;

protected:

	/**
	 * This will put the task back in the queue
	 * and will finish executing in the future.
	 *
	 * Note: This will cause run to be called again!
	 * Use state variables to make this work properly.
	 *
	 * Must be called internally, similar to return.
	 */
	void yeild();

};

class TaskEngine {

private:

	std::queue<Task*> tasks;
	std::vector<std::thread> pool;

public:

	~TaskEngine();

	/**
	 * Creates a new EventEngine with a thread pool.
	 * The size can be controlled if needed.
	 */
	TaskEngine(int poolSize = 4){};

	/**
	 * Threads run immediately.
	 */
	void addTask(Task* t);

};

}

#endif

