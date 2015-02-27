#include "ConstantVelocity.h"

ConstantVelocity::ConstantVelocity(std::weak_ptr<GameObject> go,
                                   const Ogre::Vector3& v)
  : Component(go), velocity(v) {}

void ConstantVelocity::update(const UpdateInfo& info) const {
  std::shared_ptr<GameObject> gameObjPtr = gameObject.lock();
  Ogre::Vector3 ofs = info.deltaTime * velocity;
  gameObjPtr->setPosition(gameObjPtr->getPosition() + ofs);
}
