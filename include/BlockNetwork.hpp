/*
 * BlockNetwork.hpp
 *
 *  Created on: Feb 11, 2015
 *      Author: tyler
 */

#ifndef BLOCKNETWORK_HPP_
#define BLOCKNETWORK_HPP_

#include <unordered_map>
#include <vector>
#include <string>

namespace MyEngine {

class BlockNetwork {

protected:

	std::vector<Block*> blocks;
	std::unordered_map<std::string, std::string> network;
	float power = 0;
	float powerUsed = 0;

public:

	void setValue(std::string& key, std::string& value){
		network[key] = value;
	}

	std::string& getValue(std::string& key){
		return network[key];
	}

	/**
	 * This function must account for power and any
	 * networking features a block has. Listens for
	 * block add or destroy, checks for connectivity
	 * in the network.
	 */
	void addBlock(Block* block);

};

}

#endif /* BLOCKNETWORK_HPP_ */
