/*
 * AirBlock.hpp
 *
 *  Created on: Feb 12, 2015
 *      Author: tyler
 */

#ifndef BLOCKS_AIRBLOCK_HPP_
#define BLOCKS_AIRBLOCK_HPP_

#include "Block.hpp"

namespace MyEngine {

class AirBlock : public BlockTemplate<0> {

	AirBlock(){}
	~AirBlock(){}

	static const AirBlock instance;

public:

	static Block* getSingleton(){
		return instance;
	}

};

static const AirBlock AirBlock::instance();
static const std::string BlockTemplate<0>::name = "Air";
static const float BlockTemplate<0>::hardness = std::numeric_limits<float>::infinity();

}



#endif /* BLOCKS_AIRBLOCK_HPP_ */
