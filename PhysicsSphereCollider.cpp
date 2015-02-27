#include "PhysicsSphereCollider.h"

#include <iostream>

PhysicsSphereCollider::PhysicsSphereCollider(std::weak_ptr<GameObject> go,
                                             Physics& physics,
                                             float radius,
                                             float mass)
  : PhysicsCollider(new btSphereShape(btScalar(radius)), go, physics, mass) {}

