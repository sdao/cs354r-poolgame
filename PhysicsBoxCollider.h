#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"
#include "PhysicsCollider.h"

class PhysicsBoxCollider : public PhysicsCollider {
  
  btCollisionShape* collisionShape;
  btDefaultMotionState* motionState;
  btRigidBody* rigidBody;
  GameObject* gameObject;

public: 
  PhysicsBoxCollider(GameObject* go, Physics& physics, const Ogre::Vector3& dim, float mass);
  ~PhysicsBoxCollider();
  void update() const override;

};
