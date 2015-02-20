#ifndef _gameobject_h_
#define _gameobject_h_

#include "RootGameObject.h"
#include "Component.h"

public class GameObject : RootGameObject{

	//Physics component
	//Audio component
	vector<Component*> components;
	Entity* mesh; //Entity component

	GameObject(Ogre::SceneManager*, string, string, Ogre::Vector3, Ogre::Vector3, Ogre::Vector3);

	void update();
};

#endif
