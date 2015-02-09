/*
 * Block.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: tyler
 */

#ifndef MYENGINE_BLOCK_HPP
#define MYENGINE_BLOCK_HPP

#include <vector>
#include <string>
#include <memory>

#include "Event.hpp"

namespace MyEngine {

/**
 * A block is a type of material that can hold
 * space in the world. A block when destroyed
 * becomes an item until placed again or can be
 * erased completely.
 *
 * This is the abstract (virtual) class of a block.
 * All other blocks will inherit from this including
 * its subtypes like liquids and active blocks.
 */
class Block {

protected:

	virtual ~Block(){}

public:

	virtual const uint getId() const = 0;
	virtual const std::string& getName() const = 0;

};

/**
 * A block network is a useful structure that reduces memory by linking all
 * blocks together that share a state. For instance, power levels or network
 * data. This essentially reduces each blocks state size to the network they
 * are connected to.
 */
class BlockNetwork {

protected:

	//TODO Find a way to make a singleton network manager or connect it to the world.

	std::vector<Block*> blocks; //List of blocks in the network.
	std::unordered_map<std::string, std::string> data;

public:

	/**
	 * Merges two networks together.
	 */
	BlockNetwork& merge(BlockNetwork& b);

};

class PoweredBlock : public Block {

protected:

	BlockNetwork* network;

public:

	/**
	 * Returns if the block is currently providing power.
	 */
	virtual const bool isPowered() const = 0;

	/*
	 * Returns how much power is left to be drawn on this block.
	 * This information is connected to the network of blocks and is
	 * based on the total power sources, total power usage, and size of
	 * the network.
	 */
	virtual const double powerAvailable() const = 0;

	/**
	 * Returns total power on the network.
	 */
	virtual const double totalPower() const = 0;

};

}

#endif /* MYCRAFT_BLOCK_HPP */
