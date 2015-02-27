#include "RootGameObject.h"

RootGameObject::RootGameObject(Ogre::SceneManager* sceneMgr, const std::string name){
	node = sceneMgr->getRootSceneNode()->createChildSceneNode(name + "node");
}

void RootGameObject::translate(const Ogre::Vector3& trans, const Ogre::Node::TransformSpace ts){
	node->translate(trans, ts);
}

void RootGameObject::translate(float x, float y, float z, const Ogre::Node::TransformSpace ts) {
	node->translate(x, y, z, ts);
}

void RootGameObject::setPosition(const Ogre::Vector3& position){
	node->setPosition(position);
}

const Ogre::Vector3 RootGameObject::getPosition() const {
	return node->getPosition();
}

void RootGameObject::setWorldPosition(const Ogre::Vector3& position) {
	node->_setDerivedPosition(position);
}

const Ogre::Vector3 RootGameObject::getWorldPosition() const {
	return node->_getDerivedPosition();
}

void RootGameObject::rotate(const Ogre::Vector3& axis, const Ogre::Radian& rads, const Ogre::Node::TransformSpace ts){
	node->rotate(axis, rads, ts);
}

void RootGameObject::yaw(const Ogre::Degree& deg){
	node->yaw(deg);
}

void RootGameObject::pitch(const Ogre::Degree& deg){
	node->pitch(deg);
}

void RootGameObject::roll(const Ogre::Degree& deg){
	node->roll(deg);
}

void RootGameObject::scale(const Ogre::Vector3& scale){
	node->scale(scale);
}

void RootGameObject::setScale(const Ogre::Vector3& scale){
	node->setScale(scale);
}

		
Ogre::Vector3 RootGameObject::localToWorldPosition(const Ogre::Vector3& v)
	const
{
	return node->convertLocalToWorldPosition(v);
}
Ogre::Vector3 RootGameObject::worldToLocalPosition(const Ogre::Vector3& v)
	const
{
	return node->convertWorldToLocalPosition(v);
}
