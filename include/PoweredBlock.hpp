/*
 * PoweredBlock.hpp
 *
 *  Created on: Feb 12, 2015
 *      Author: tyler
 */

#ifndef POWEREDBLOCK_HPP_
#define POWEREDBLOCK_HPP_


namespace MyEngine {

class PoweredBlock : public Block {

public:

	/**
	 * Either positive or negative for power usage or
	 * how much power it provides.
	 */
	virtual const float getPower() const = 0;

	/**
	 * Returns maximum amount of power the node can allow
	 * to pass through itself. If the power goes over this
	 * level, it will explode.
	 */
	virtual const float getMaxPower() const = 0;

	/**
	 * This is the minimum amount of power required to operate.
	 */
	virtual const float getMinPower() const = 0;

};

template<uint ID>
class PoweredBlockTemplate : public PoweredBlock {

private:

	static const float power;
	static const float minPower;
	static const float maxPower;

public:

	virtual const float getPower() const {
		return power;
	}

	virtual const float getMaxPower() const {
		return maxPower;
	}

	virtual const float getMinPower() const {
		return minPower;
	}

};

}


#endif /* POWEREDBLOCK_HPP_ */
