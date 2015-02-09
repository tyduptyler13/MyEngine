/*
 * Region.hpp
 *
 *  Created on: Feb 8, 2015
 *      Author: tyler
 */

#ifndef REGION_HPP_
#define REGION_HPP_

/**
 * A region is an area in the world that allows for non standard placement of blocks.
 * For instance, a rotating dome over a city would have a region mounted
 * to the center point of the city and the dome blocks would exist in the region instead
 * of the chunks of the city. Regions are attached by mechanical blocks. Mechanical
 * blocks define the physics of a region which can be detached making the blocks a vehicle
 * or can be attached to a block in a chunk allowing the blocks to be rotated or moved.
 *
 * Ideas for mechanisms:
 * * Conveyer belt - Move along belt (trains could use specialized versions of this)
 * 	 * Magnetic levitation system - Allow conveyer belt to be on walls (rail gun possibilities)
 * * Hydraulic arm - Rotate and pivot
 * * Hinge - Pivot/Rotate on an axis
 */

namespace MyEngine {

class Region {

};

}



#endif /* REGION_HPP_ */
