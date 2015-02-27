#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"
#include "PhysicsCollider.h"
#include <memory>

class PhysicsBoxCollider : public PhysicsCollider {
public: 
  PhysicsBoxCollider(std::weak_ptr<GameObject> go,
                     Physics& physics,
                     const Ogre::Vector3& dim,
                     float mass);
};
