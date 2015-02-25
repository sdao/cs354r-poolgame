#include "ConstantVelocity.h"

ConstantVelocity::ConstantVelocity(std::shared_ptr<GameObject> go,
                                   const Ogre::Vector3& v)
  : gameObject(go), velocity(v) {}

void ConstantVelocity::update(float deltaTime) const {
  Ogre::Vector3 ofs = deltaTime * velocity;
  gameObject->setPosition(gameObject->getPosition() + ofs);
}
