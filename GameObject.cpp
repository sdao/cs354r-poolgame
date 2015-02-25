#include "GameObject.h"

GameObject::GameObject(Ogre::SceneManager* mSceneMgr,
                       const std::string& meshname,
                       const std::string& entityname) : 
			RootGameObject(mSceneMgr, entityname)
{
	mesh = mSceneMgr->createEntity(entityname, meshname);
	node->attachObject(mesh);
}

GameObject::GameObject(Ogre::SceneManager* mSceneMgr,
                       const std::string& meshname,
                       const std::string& entityname,
                       const Ogre::Vector3& position = Ogre::Vector3::ZERO,
                       const Ogre::Vector3& rotation = Ogre::Vector3::ZERO,
                       const Ogre::Vector3& scale = Ogre::Vector3(1, 1, 1)) : 
			RootGameObject(mSceneMgr, entityname)
{
	mesh = mSceneMgr->createEntity(entityname, meshname);
	node->attachObject(mesh);

	setPosition(position);
	rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rotation.x));
	rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(rotation.y));
	rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rotation.z));
	setScale(scale);
}

void GameObject::update() {
	for (Component* comp : components) {
	  comp->update();
	}
}

void GameObject::setMaterial(std::string& name){
	mesh->setMaterialName(name);
}
