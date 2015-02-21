#ifndef _gameobject_h_
#define _gameobject_h_

#include <vector>
#include <OgreEntity.h>

//#include "RootGameObject.h"
#include "Component.h"

class GameObject /* : RootGameObject */ {

	std::vector<Component*> components; // List of generic components.
	Ogre::Entity* mesh; //Entity component

	GameObject(Ogre::SceneManager*,
	           std::string,
	           std::string,
	           Ogre::Vector3,
	           Ogre::Vector3,
	           Ogre::Vector3);

	void update();
};

#endif
