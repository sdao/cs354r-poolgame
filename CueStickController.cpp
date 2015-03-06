#include "CueStickController.h"
#include <iostream>

#include "PhysicsCollider.h"
#include "ObjectSound.h"

CueStickController::CueStickController(std::weak_ptr<GameObject> go)
  : Component(go), currentObj() {}

void CueStickController::update(const UpdateInfo& update) {
  std::shared_ptr<GameObject> gameObjPtr = gameObject.lock();
  Ogre::Vector3 goWorldPosition =
    gameObjPtr->localToWorldPosition(Ogre::Vector3::ZERO);
  Ogre::Vector3 targetWorldPosition =
    gameObjPtr->localToWorldPosition(Ogre::Vector3::UNIT_Y);
  Ogre::Vector3 dirGoToTarget = targetWorldPosition - goWorldPosition;
  auto colliders = update.physics->rayCastAll(goWorldPosition,
                                              dirGoToTarget,
                                              10000.0f);
  for (PhysicsCollider* collider : colliders) {
    std::weak_ptr<GameObject> colliderObj = collider->getGameObject();
    std::shared_ptr<GameObject> colliderObjPtr = colliderObj.lock();
    std::shared_ptr<GameObject> currentObjPtr = currentObj.lock();

    if (colliderObjPtr != currentObjPtr) {
      std::shared_ptr<GameObject> currentObjPtr = currentObj.lock();
      
      if (currentObjPtr) {
        currentObjPtr->setMaterial("Examples/Chrome");
      }
      currentObj = std::weak_ptr<GameObject>();
    }

    if (colliderObjPtr->getTag() == 0x1) {
      //std::cout << "saw a ball!\n";
      currentObj = colliderObj;
      colliderObjPtr->setMaterial("Examples/GrassFloor");
      return;
    }
  }
  
  //std::cout << "NADA :(\n";
}

void CueStickController::hit(std::string strength) {
  std::shared_ptr<GameObject> gameObjPtr = gameObject.lock();
  std::shared_ptr<GameObject> currentObjPtr = currentObj.lock();
  float power;
  if (strength == "Low")
    power = 1600.0;
  else if (strength == "Medium")
    power = 3200.0;
  else if (strength == "High")
    power = 4800.0;
  if (currentObjPtr) {
    auto collider = currentObjPtr->getComponent<PhysicsCollider>();
    if (collider) {
      Ogre::Vector3 goWorldPosition =
        gameObjPtr->localToWorldPosition(Ogre::Vector3::ZERO);
      Ogre::Vector3 targetWorldPosition =
        gameObjPtr->localToWorldPosition(Ogre::Vector3::UNIT_Y);
      Ogre::Vector3 dirGoToTarget = (targetWorldPosition - goWorldPosition)
        .normalisedCopy();
      collider->applyWorldImpulse(dirGoToTarget * power);
      //std::cout << dirGoToTarget.x << "  << dirGoToTarget.y << " " << dirGoToTarget.z << "\n";
      auto sound = gameObjPtr->getComponent<ObjectSound>();
      if (sound) {
        sound->collision();
      }
    }
  }
}

