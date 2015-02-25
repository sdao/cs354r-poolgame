#pragma once

#include "Component.h"
#include "GameObject.h"
#include <memory>

class ConstantVelocity : public Component {
  std::shared_ptr<GameObject> gameObject;
  Ogre::Vector3 velocity;

public:
  ConstantVelocity(std::shared_ptr<GameObject> go, const Ogre::Vector3& v);
  virtual void update(float deltaTime) const override;
};

