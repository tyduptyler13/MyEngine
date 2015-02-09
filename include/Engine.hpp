/*
 * Engine.hpp
 *
 *  Created on: Jan 27, 2015
 *      Author: tyler
 */

#ifndef MYENGINE_ENGINE_HPP
#define MYENGINE_ENGINE_HPP

#include <string>
#include <memory>
#include <exception>

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreOverlaySystem.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

namespace MyEngine {

class Engine {

protected:

	std::unique_ptr<Ogre::Root> root;
	std::unique_ptr<Ogre::RenderWindow> window;
	std::unique_ptr<Ogre::Camera> camera;
	std::unique_ptr<Ogre::SceneManager> scene;

	//Missing?
	std::unique_ptr<Ogre::OverlaySystem> overlay;

	std::unique_ptr<OIS::InputManager> inputManager;
	std::unique_ptr<OIS::Mouse> mouse;
	std::unique_ptr<OIS::Keyboard> keyboard;

public:

	/**
	 * Basic design before it is reworked to be more flexible in the future.
	 * This will allow for some quick implementation and testing.
	 * Taken directly from the ogre tutorial.
	 */
	Engine(std::string plugins = "plugins.cfg", std::string name){
		root = new Ogre::Root(plugins);

		Ogre::ConfigFile configFile;
		configFile.load(plugins);

		Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements()){
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i){
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
						archName, typeName, secName);
			}
		}

		if (root->restoreConfig() || root->showConfigDialog()){
			window = root->initialise(true, "MyEngine");
		} else {
			throw std::exception("Failed to create a new window.");
		}

		scene = root->createSceneManager(Ogre::ST_GENERIC);

		overlay = new Ogre::OverlaySystem();
		scene->addRenderQueueListener(overlay.get());

		camera = scene->createCamera("PlayerCam");
		camera->setPosition(Ogre::Vector3(0,0,80));
		camera->lookAt(Ogre::Vector3(0,0,-300));
		camera->setNearClipDistance(5);

		Ogre::Viewport* vp = window->addViewport(camera.get());
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

		camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList p1;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		window->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		p1.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		inputManager = OIS::InputManager::createInputSystem(p1);

		keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
		mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));



		//Allow the creator to handle loading stuff.
	}



};

}

#endif
