#include "CueStickController.h"
#include <iostream>

#include "PhysicsCollider.h"

CueStickController::CueStickController(std::weak_ptr<GameObject> go)
  : Component(go), currentObj() {}

void CueStickController::update(const UpdateInfo& update) {
  std::shared_ptr<GameObject> gameObjPtr = gameObject.lock();
  Ogre::Vector3 goWorldPosition =
    gameObjPtr->localToWorldPosition(Ogre::Vector3::ZERO);
  Ogre::Vector3 targetWorldPosition =
    gameObjPtr->localToWorldPosition(Ogre::Vector3::UNIT_Y);
  Ogre::Vector3 dirGoToTarget = targetWorldPosition - goWorldPosition;
  PhysicsCollider* collider = update.physics->rayCast(goWorldPosition,
                                                      dirGoToTarget,
                                                      10000.0f);
  if (collider) {
    std::weak_ptr<GameObject> colliderObj = collider->getGameObject();
    std::shared_ptr<GameObject> colliderObjPtr = colliderObj.lock();
    std::shared_ptr<GameObject> currentObjPtr = currentObj.lock();

    if (colliderObjPtr != currentObjPtr) {
      std::shared_ptr<GameObject> currentObjPtr = currentObj.lock();
      
      if (currentObjPtr) {
        currentObjPtr->setMaterial("Examples/BumpyMetal");
      }
      currentObj = std::weak_ptr<GameObject>();
    }

    if (colliderObjPtr->getTag() & 0x1 == 1) {
      //std::cout << "saw a ball!\n";
      currentObj = colliderObj;
      colliderObjPtr->setMaterial("Examples/GrassFloor");
      return;
    }
  }
  
  //std::cout << "NADA :(\n";
}

void CueStickController::hit() {
  std::shared_ptr<GameObject> gameObjPtr = gameObject.lock();
  std::shared_ptr<GameObject> currentObjPtr = currentObj.lock();
  if (currentObjPtr) {
    auto collider = currentObjPtr->getComponent<PhysicsCollider>();
    if (collider) {
      Ogre::Vector3 goWorldPosition =
        gameObjPtr->localToWorldPosition(Ogre::Vector3::ZERO);
      Ogre::Vector3 targetWorldPosition =
        gameObjPtr->localToWorldPosition(Ogre::Vector3::UNIT_Y);
      Ogre::Vector3 dirGoToTarget = targetWorldPosition - goWorldPosition;
      collider->applyWorldImpulse(dirGoToTarget * 1000.0f);
      std::cout << dirGoToTarget.x << " " << dirGoToTarget.y << " " << dirGoToTarget.z << "\n";
    }
  }
}

