#ifndef MYUPLAY_MYENGINE_EVENTDISPATCHER
#define MYUPLAY_MYENGINE_EVENTDISPATCHER

#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>
#include <algorithm>

namespace MyUPlay {

	namespace MyEngine {

		/// This class is meant to be inherited by subclasses to
		/// Naturally inherit is event methods. A class simply needs
		/// to define two things: One, is its own type so that all return
		/// statements return things without casting, two, is a list of
		/// valid events that can be passed to the methods in the form
		/// of an enum.
		template <typename ObjectType, typename Event>
		class EventDispatcher {

		public:
			typedef std::function<void(ObjectType&)> Listener;
			typedef std::shared_ptr<Listener> SharedListener;

		private:
			std::unordered_map<Event, std::vector<SharedListener>> listeners;

		public:

			void addEventListener(Event type, SharedListener& l){

				if (listeners.find(type) == listeners.end() ) {
					listeners.emplace(std::vector<SharedListener>());
				}

				listeners[type].push(l);

			}

			bool hasEventListener(Event type, SharedListener& l){
				if (listeners.find(type) == listeners.end()){
					return false;
				}

				std::vector<SharedListener>& list = listeners[type];
				if (std::find(list.begin(), list.end(), l) != list.end()){
					return true;
				}

				return false;

			}

			void removeEventListener(Event type, SharedListener& l){

				if (listeners.find(type) == listeners.end()){
					return;
				}

				std::vector<SharedListener>& list = listeners[type];
				auto pointer = std::find(list.begin(), list.end(), l);
				if (pointer != list.end()){
					list.erase(pointer);
				}

			}

			void dispatchEvent(Event type, ObjectType& ref){

				if (listeners.find(type) == listeners.end()){
					return;
				}

				for (SharedListener& l : listeners[type]){
					(*l)(ref);
				}

			}

		};

	}

}


#endif


