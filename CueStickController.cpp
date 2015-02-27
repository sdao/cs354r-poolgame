#include "CueStickController.h"
#include <iostream>

#include "PhysicsCollider.h"

CueStickController::CueStickController(std::weak_ptr<GameObject> go)
  : Component(go) {}

void CueStickController::update(const UpdateInfo& update) const {
  std::shared_ptr<GameObject> gameObjPtr = gameObject.lock();
  Ogre::Vector3 goWorldPosition =
    gameObjPtr->localToWorldPosition(Ogre::Vector3::ZERO);
  Ogre::Vector3 targetWorldPosition =
    gameObjPtr->localToWorldPosition(Ogre::Vector3::UNIT_Z);
  Ogre::Vector3 dirGoToTarget = targetWorldPosition - goWorldPosition;
  PhysicsCollider* collider = update.physics->rayCast(goWorldPosition,
                                                      dirGoToTarget,
                                                      10000.0f);
  if (collider) {
    std::cout << "I see something!\n";
  } else {
    std::cout << "NADA :(\n";
  }
}
