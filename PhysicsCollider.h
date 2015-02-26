#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

class PhysicsCollider : public Component {
protected:
  btCollisionShape* collisionShape;
  btDefaultMotionState* motionState;
  btRigidBody* rigidBody;
  std::shared_ptr<GameObject> gameObject;
  bool isDynamic;

public:
  PhysicsCollider(btCollisionShape* cs,
                  std::shared_ptr<GameObject> go,
                  Physics& physics,
                  float mass = 1.0f,
                  float friction = 0.5f);
  virtual ~PhysicsCollider();
  void update(float deltaTime) const;
  virtual void didCollide(const PhysicsCollider& other) const;

};
