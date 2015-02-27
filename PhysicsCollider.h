#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

class PhysicsCollider : public Component {
protected:
  btCollisionShape* collisionShape;
  btDefaultMotionState* motionState;
  btRigidBody* rigidBody;
  bool isDynamic;

public:
  /**
   * Creates a new physics collider.
   *
   * @param go       the object driven by the collider
   * @param physics  the physics world
   * @param mass     the mass of the object (kg); note that
   *                 mass <= 0.0f makes it kinematic, not dynamic
   * @param friction the friction coefficient of the object
   */
  PhysicsCollider(btCollisionShape* cs,
                  std::weak_ptr<GameObject> go,
                  Physics& physics,
                  float mass = 1.0f,
                  float friction = 0.5f);
  virtual ~PhysicsCollider();
  void update(const UpdateInfo& info) const;
  virtual void didCollide(const PhysicsCollider& other) const;

  /**
   * Applies a force with the given world-space vector to the collider.
   */
  void applyWorldForce(const Ogre::Vector3& force);

  /**
   * Applies an impulse (force over time) with the given world-space
   * vector to the collider.
   */
  void applyWorldImpulse(const Ogre::Vector3& impulse);

};
