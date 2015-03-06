#include "GoalController.h"
#include "PhysicsCollider.h"
#include <iostream>

void GoalController::didCollide(PhysicsCollider& collider) {
  // Hide the other object's geometry.
  auto otherGo = collider.getGameObject();
  auto otherGoPtr = otherGo.lock();
  if (otherGoPtr->getTag() == 0x2 /* non-cue ball */) {
    otherGoPtr->setTag(0x0);
    otherGoPtr->setVisible(false);
    collider.removeFromPhysics();
    std::cout << "!!!!!!!!!!!\n";
    std::cout << "wow, you got one in\n";
    std::cout << "!!!!!!!!!!!\n";
  } else if (otherGoPtr->getTag() == 0x1 /* cue ball */) {
    std::cout << "***********\n";
    std::cout << "you suck   \n";
    std::cout << "***********\n";
  }
}

