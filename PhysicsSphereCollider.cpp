#include "PhysicsSphereCollider.h"

#include <iostream>

PhysicsSphereCollider::PhysicsSphereCollider(std::weak_ptr<GameObject> go,
                                             Physics& physics,
                                             float radius,
                                             float mass,
                                             bool trigger)
  : PhysicsCollider(new btSphereShape(btScalar(radius)),
                    go,
                    physics,
                    mass,
                    20.0f,
                    trigger) {}

