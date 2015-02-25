#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"
#include "PhysicsCollider.h"
#include <memory>

class PhysicsBoxCollider : public PhysicsCollider {
  
  btCollisionShape* collisionShape;
  btDefaultMotionState* motionState;
  btRigidBody* rigidBody;
  std::shared_ptr<GameObject> gameObject;
  bool isDynamic;

public: 
  PhysicsBoxCollider(std::shared_ptr<GameObject> go, Physics& physics, const Ogre::Vector3& dim, float mass);
  ~PhysicsBoxCollider();
  void update(float deltaTime) const override;

};
