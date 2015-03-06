#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

class PhysicsCollider : public Component {
protected:
  btCollisionShape* collisionShape;
  btDefaultMotionState* motionState;
  btRigidBody* rigidBody;
  Physics* physics;
  bool isDynamic;
  bool isTrigger;
  float collisionTimer;
  bool inCollision;
  int collisionId;
  static constexpr float COLLISION_REPORT_DELAY = 0.2f;

public:
  /**
   * Creates a new physics collider, automatically adding it to the specified
   * physics world.
   *
   * @param go       the object driven by the collider
   * @param physics  the physics world
   * @param mass     the mass of the object (kg); note that
   *                 mass <= 0.0f makes it kinematic, not dynamic
   * @param friction the friction coefficient of the object
   * @param trigger  whether the collider should be a trigger instead of
   *                 producing a collision response
   */
  PhysicsCollider(btCollisionShape* cs,
                  std::weak_ptr<GameObject> go,
                  Physics& phys,
                  float mass = 1.0f,
                  float friction = 20.0f,
                  bool trigger = false);
  virtual ~PhysicsCollider();
  void update(const UpdateInfo& info);

  /**
   * Used by the physics system to report that a collision has begun between
   * the current collider and some other collider. This event will probably
   * only happen once during the collision.
   *
   * @param other a reference to the other collider
   * @param time  the current system time at point of collision
   */
  virtual void reportCollision(PhysicsCollider& other, float time);

  /**
   * Applies a force with the given world-space vector to the collider.
   */
  void applyWorldForce(const Ogre::Vector3& force);

  /**
   * Applies an impulse (force over time) with the given world-space
   * vector to the collider.
   */
  void applyWorldImpulse(const Ogre::Vector3& impulse);

  /** Removes the collider from its current physics world. */
  void removeFromPhysics();

  /** Adds the collider to the specified physics world. */
  void addToPhysics(Physics& phys);

  /** Gets the current scalar speed of the collider. */
  float getVelocity() const;

};
