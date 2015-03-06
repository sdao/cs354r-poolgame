#include "GoalController.h"
#include "PhysicsCollider.h"
#include "ObjectSound.h"
#include <iostream>

GoalController::GoalController(std::shared_ptr<GameInfo> ginfo)
	: Component()
{
	gameinfo = ginfo;
}

void GoalController::didCollide(PhysicsCollider& collider) {
  auto otherGo = collider.getGameObject();
  auto otherGoPtr = otherGo.lock();

  auto otherGoSound = otherGoPtr->getComponent<ObjectSound>();

  if (otherGoPtr->getTag() == 0x2 /* non-cue ball */) {
    // Hide the other object's geometry.
    otherGoPtr->setTag(0x0);
    otherGoPtr->setVisible(false);
    collider.removeFromPhysics();

    if (otherGoSound) {
      otherGoSound->collision(2);
    }

    std::cout << "!!!!!!!!!!!\n";
    std::cout << "wow, you got one in\n";
    std::cout << "!!!!!!!!!!!\n";
  } else if (otherGoPtr->getTag() == 0x1 /* cue ball */) {
    if (otherGoSound) {
      otherGoSound->collision(3);
    }

    std::cout << "*************************\n";
    std::cout << "you suck. MINUS 1 FOR YOU   \n";
    std::cout << "*************************\n";
    gameinfo->scoreP1 -= 1;
  }
}

