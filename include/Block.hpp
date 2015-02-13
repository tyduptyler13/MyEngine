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
#include <limits>

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

	/**
	 * Returns the identifying id of the block.
	 */
	virtual const uint getId() const = 0;

	/**
	 * Returns the name of the block, usually for human readability.
	 */
	virtual const std::string& getName() const = 0;

	/**
	 * Returns the hardness of a block. This affects how hard
	 * it is to mine and how easily it can be destroyed.
	 */
	virtual const float getHardness() const = 0;

	/**
	 * This function is effectively delete. Functions that need
	 * to be deleted should override this. Most blocks won't need to
	 * as most blocks are static singleton blocks.
	 */
	virtual void cleanUp(){};

};

template <uint ID>
class BlockTemplate : public Block {

protected:

	static const uint id = ID;
	static const std::string name;
	static const float hardness;

public:

	const uint getId() const {
		return id;
	}

	const std::string& getName() const {
		return name;
	}

	const float getHardness() const {
		return hardness;
	}

};

}

#endif /* MYCRAFT_BLOCK_HPP */
