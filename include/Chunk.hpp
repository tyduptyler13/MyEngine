/*
 * Chunk.hpp
 *
 *  Created on: Jan 27, 2015
 *      Author: tyler
 */

#ifndef MYENGINE_CHUNK_HPP
#define MYENGINE_CHUNK_HPP

#include <fstream>
#include <mutex>
#include <functional>

#include "World.hpp"
#include "Vector3.hpp"
#include "Block.hpp"

//Ensure this is always a power of 2 for fast math.
#define CHUNK_SIZE 32

namespace MyEngine {

/**
 * Blocks are stored in a chunk to allow for segmentation
 * of large areas of blocks. This is efficient for memory
 * retrieval.
 */
class Chunk {

protected:

	//Marks the world that the chunk exists in.
	World* world;

	const Vector3<int> location;

	//Handle multiple threads accessing chunks.
	mutable std::mutex lock;

	//The list of all blocks.
	Block const ** blocks;

	~Chunk(){
		for (Block* block : blocks){
			block->cleanUp();
		}

		delete[] blocks;
	}

public:

	Chunk(World* world, Vector3<int> location) : world(world), location(location){
		blocks = new Block*[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE](); //Note the () clears the memory.
	}

	Chunk(World* world, Vector3<int> location, std::ifstream file) : world(world), location(location){

		blocks = new Block*[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE]; //No memory cleanup.

		//TODO Read chunk in from file.

	}

	/**
	 * Returns a block based on a location in the chunk.
	 * The blocks are oriented so that the blocks count up
	 * based on the most negative corner of the chunk. Consider
	 * this location the origin of the chunk and it starts with
	 * 0,0,0
	 */
	Block* const getBlock(uint x, uint y, uint z) const {
		lock.lock();
		Block* b = blocks[x + CHUNK_SIZE * y + CHUNK_SIZE * CHUNK_SIZE * z];
		lock.unlock();
		return b;
	}

	void setBlock(Block* b, uint x, uint y, uint z) {
		lock.lock();
		Block* bl = blocks[x + CHUNK_SIZE * y + CHUNK_SIZE * CHUNK_SIZE * z];
		bl->cleanUp();
		bl = b;
		lock.unlock();
	}

	const Vector3<int>& getLocation() const {
		return location;
	}

	/**
	 * Allows any thread to perform exclusive actions on the chunk.
	 * Notice that if this function never returns then you have dead
	 * locked a chunk and it cannot progress.
	 */
	void exclusive(std::function<void(Chunk&)> func){
		lock.lock();
		func(*this);
		lock.unlock();
	}

	/**
	 * Returns the most negative corner of the block relative to the world.
	 */
	const Vector3<int> getBlockLocation(uint x, uint y, uint z) const {
		return Vector3<int>(x + location.x, y + location.y, z + location.z);
	}

	/**
	 * Returns a block relative to its global location.
	 */
	Block* getBlockByLocation(Vector3<int>& location) const {
		return getBlockByLocation(location.x, location.y, location.z);
	}

	Block* getBlockByLocation(int x, int y, int z) const {

		uint lx = x - location.x;
		uint ly = y - location.y;
		uint lz = z - location.z;

		if (lx >= 32 || ly >= 32 || lz >= 32){
			throw std::exception("Out of bounds error.");
		}

		lock.lock();

		Block* b = blocks[lx + CHUNK_SIZE * ly + CHUNK_SIZE * CHUNK_SIZE * lz];

		lock.unlock();

		return b;

	}

};

}


#endif
