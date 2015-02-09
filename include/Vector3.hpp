/*
 * Vector3.hpp
 *
 *  Created on: Feb 8, 2015
 *      Author: tyler
 */

#ifndef VECTOR3_HPP_
#define VECTOR3_HPP_

namespace MyEngine {

/**
 * Extremely basic vector3. Primarily for storage, not for math.
 * If you need a math vector, then use Ogre::Vector3
 */
template <typename T>
class Vector3 {

protected:

	T x, y, z;

public:

	T getX(){
		return x;
	}

	T getY(){
		return y;
	}

	T getZ(){
		return z;
	}

	void set(T x, T y, T z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void setX(T x){
		this->x = x;
	}

	void setY(T y){
		this->y = y;
	}

	void setZ(T z){
		this->z = z;
	}

};

}

#endif /* VECTOR3_HPP_ */
