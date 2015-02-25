#include "GameObject.h"
#include <iostream>

/*
GameObject::GameObject(Ogre::SceneManager* mSceneMgr,
                       const std::string& meshname,
                       const std::string& entityname) : 
			RootGameObject(mSceneMgr, entityname)
{
	mesh = mSceneMgr->createEntity(entityname, meshname);
	node->attachObject(mesh);
}
*/

GameObject::GameObject(Ogre::SceneManager* mSceneMgr,
                       const std::string& meshname,
                       const std::string& entityname,
                       const Ogre::Vector3& position,
                       const Ogre::Vector3& rotation,
                       const Ogre::Vector3& scale) : 
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

GameObject::GameObject(Ogre::SceneManager* mSceneMgr,
                       Ogre::Entity* entity,
                       const std::string& name,
                       const Ogre::Vector3& position,
                       const Ogre::Vector3& rotation,
                       const Ogre::Vector3& scale) : 
		       RootGameObject(mSceneMgr, name), mesh(entity)
{
	node->attachObject(mesh);

	setPosition(position);
	rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rotation.x));
	rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(rotation.y));
	rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rotation.z));
	setScale(scale);
}


void GameObject::update() {
	for (auto comp : components) {
	  comp->update();
	}
}

void GameObject::setMaterial(const std::string& name){
	mesh->setMaterialName(name);
}

void GameObject::addComponent(std::shared_ptr<Component> comp) {
	components.push_back(comp);
}

