/*
 * Chunk.hpp
 *
 *  Created on: Jan 27, 2015
 *      Author: tyler
 */

#ifndef MYENGINE_CHUNK_HPP
#define MYENGINE_CHUNK_HPP

#include "World.hpp"

namespace MyEngine {

class Chunk {

protected:

	//Marks the world that the chunk exists in.
	World* world;

	Vector3<int> location;

public:


};

}


#endif
