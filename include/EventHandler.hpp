
#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

namespace MyEngine {

/**
 * Interface for event handlers.
 *
 * They must define the type of event they are handling.
 */
template < class T >
class EventHandler {

    virtual ~EventHandler(){};

public:

    class Test : public T{

    };


    virtual void handle(T event) = 0;

};

}

#endif
