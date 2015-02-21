#include "GameObject.h"

GameObject::GameObject(Ogre::SceneManager* mSceneMgr,
                       std::string meshname,
                       std::string entityname,
                       Ogre::Vector3 position,
                       Ogre::Vector3 rotation,
                       Ogre::Vector3 scale)
{
	
}

void GameObject::update() {
	for (Component* comp : components) {
	  comp->update();
	}
}
