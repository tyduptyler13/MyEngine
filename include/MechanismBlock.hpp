/*
 * MechansimBlock.hpp
 *
 *  Created on: Feb 12, 2015
 *      Author: tyler
 */

#ifndef MECHANISMBLOCK_HPP_
#define MECHANISMBLOCK_HPP_

#include <OgreVector3.h>

#include "Region.hpp"

namespace MyEngine {

/**
 *  This is a special instance of a block that allows for
 *  a region to be attached to it based on custom rules dictating
 *  out the region is supposed to be oriented.
 */
class MechanismBlock {

private:

	World* world;
	Region region;

	/**
	 * Offset and rotation of regions. This includes all
	 * of the glorious math functionality of Ogre.
	 */
	Ogre::Vector3 offset;
	Ogre::Vector3 rotation;

public:

	/**
	 * This block must register itself to be updated when the engine ticks.
	 */
	MechanismBlock();

};

}

#endif /* MECHANISMBLOCK_HPP_ */
