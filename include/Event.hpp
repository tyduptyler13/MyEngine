#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>

#include "EventHandler.hpp"

namespace MyEngine {

/**
 * Subclasses of this should extend Event<Subclass>.
 * This allows for static polymorphism aka much less work.
 */
template <class T>
class Event {
private:

	virtual ~Event(){};

	static std::vector<EventHandler<T>> handlers;

	bool canceled;
	bool consumed;

public:

	/**
	 * Adds an event handler to the templated event.
	 */
	static void addHandler(EventHandler<T>& h);
	static void removeHandler(EventHandler<T>& h);

	void fire(TaskEngine& e){
		e.addTask(this);
	}

};

}

#endif

