/*
 * Engine.hpp
 *
 *  Created on: Jan 27, 2015
 *      Author: tyler
 */

#ifndef MYENGINE_ENGINE_HPP
#define MYENGINE_ENGINE_HPP

#include <string>

#include "OgreRoot.h"
#include "OgreConfigFile.h"



namespace MyEngine {

class Engine {

public:

	Ogre::Root root;

	Engine(std::string plugins = "plugins.cfg");


};

}

#endif
