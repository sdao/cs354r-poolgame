#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"
#include "PhysicsCollider.h"

class PhysicsSphereCollider : public PhysicsCollider {
  
  btCollisionShape* collisionShape;
  btDefaultMotionState* motionState;
  btRigidBody* rigidBody;
  GameObject* gameObject;
  
  PhysicsSphereCollider(GameObject* go, Physics& physics, float radius, float mass);
  ~PhysicsSphereCollider();
  void update() const override;

};
