/*
 * Block.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: tyler
 */

#ifndef MYCRAFT_BLOCK_HPP
#define MYCRAFT_BLOCK_HPP

#include <string>

namespace MyCraft {

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

private:

	virtual ~Block(){}

public:

	uint getType() = 0;
	virtual std::string& getName() = 0;

};

template <uint id>
class BlockInterface : public Block {

private:

	virtual ~BlockInterface(){}

public:

	static const uint type = id;
	static const std::string name;

	uint getType(){
		return type;
	}

	std::string& getName(){
		return name;
	}

};

}

#endif /* MYCRAFT_BLOCK_HPP */
