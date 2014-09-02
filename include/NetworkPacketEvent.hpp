/*
 * NetworkPacketEvent.hpp
 *
 *  Created on: Aug 27, 2014
 *      Author: tyler
 */

#ifndef NETWORKPACKETEVENT_HPP_
#define NETWORKPACKETEVENT_HPP_

#include <string>

#include "Event.hpp"

namespace MyEngine {

class NetworkPacketEvent : public Event<NetworkPacketEvent> {

	std::string data;

public:

	const unsigned long size = 0;
	const unsigned long packetId = 0;

	NetworkPacketEvent(unsigned long size, unsigned long packetId)
	: size(size), packetId(packetId) {}

	const std::string& getData() const {
		return data;
	}

};

}

#endif /* NETWORKPACKETEVENT_HPP_ */
