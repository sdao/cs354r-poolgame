#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

class PhysicsSphereCollider : public Component {
  
  btCollisionShape* collisionShape;
  btDefaultMotionState* motionState;
  btRigidBody* rigidBody;
  
  PhysicsSphereCollider(GameObject& go, Physics& physics, float radius, float mass);
  ~PhysicsSphereCollider();
  void update() const;
  
};
