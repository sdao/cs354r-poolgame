#include "PhysicsBoxCollider.h"

PhysicsBoxCollider::PhysicsBoxCollider(std::weak_ptr<GameObject> go,
                                       Physics& physics,
                                       const Ogre::Vector3& dim,
                                       float mass,
                                       bool trigger)
  : PhysicsCollider(
      new btBoxShape(btVector3(dim.x * 0.5f, dim.y * 0.5f, dim.z * 0.5f)),
      go, physics, mass, 40.0f, trigger
    ) {}

