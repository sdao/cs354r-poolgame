#include "PhysicsCollider.h"
#include <iostream>

PhysicsCollider::~PhysicsCollider() {}

void PhysicsCollider::didCollide(const PhysicsCollider& other) const {
  // TODO: have some default collision behavior.
  std::cout << "BAM!\n";
}

