#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>

#include "EventHandler.hpp"

namespace MyEngine {

class Event {

private:
	virtual ~Event(){};

protected:

	bool cancelled = false;
	bool uncancelable = false;

public:
	Event(){};

	virtual bool cancel(){
		if (!uncancelable){
			cancelled = true;
		}
	}

};

}

#endif

