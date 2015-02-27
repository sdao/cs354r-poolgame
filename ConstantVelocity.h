#pragma once

#include "Component.h"
#include "GameObject.h"
#include <memory>

class ConstantVelocity : public Component {
  Ogre::Vector3 velocity;

public:
  ConstantVelocity(std::weak_ptr<GameObject> go, const Ogre::Vector3& v);
  virtual void update(const UpdateInfo& info) const override;
};

