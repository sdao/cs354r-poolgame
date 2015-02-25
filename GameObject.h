#ifndef _gameobject_h_
#define _gameobject_h_

#include <vector>
#include <OgreEntity.h>

#include "RootGameObject.h"
#include "Component.h"

class GameObject : RootGameObject {

	protected:
		std::vector<Component*> components; // List of generic components.
		Ogre::Entity* mesh; //Entity component

	public:
		GameObject(Ogre::SceneManager*,
			   const std::string&,
			   const std::string&);

		GameObject(Ogre::SceneManager*,
		           const std::string&,
		           const std::string&,
		           const Ogre::Vector3&,
		           const Ogre::Vector3&,
		           const Ogre::Vector3&);

		void update();
		void setMaterial(std::string&);
};

#endif
