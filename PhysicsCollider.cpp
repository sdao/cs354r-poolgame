#include "PhysicsCollider.h"
#include <iostream>
#include <cassert>

PhysicsCollider::~PhysicsCollider() {
  delete rigidBody;
  delete motionState;
  delete collisionShape;
}

PhysicsCollider::PhysicsCollider(btCollisionShape* cs,
                                 std::weak_ptr<GameObject> go,
                                 Physics& physics,
                                 float mass,
                                 float friction)
  : collisionShape(cs), isDynamic(mass > 0.0f), Component(go)
{
  // Need to take temporary ownership of go.
  std::shared_ptr<GameObject> gameObjPtr = go.lock();
  assert(gameObjPtr);

  btVector3 localInertia(0, 0, 0);
  if (isDynamic) {  
    collisionShape->calculateLocalInertia(mass, localInertia);
  }

  Ogre::Vector3 objPosition = gameObjPtr->getWorldPosition();
  btTransform startTransform;
  startTransform.setIdentity();
  startTransform.setOrigin(
    btVector3(objPosition.x, objPosition.y, objPosition.z)
  );
    
  motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
                                                  motionState,
                                                  collisionShape,
                                                  localInertia);
  rbInfo.m_friction = friction;
  rigidBody = new btRigidBody(rbInfo);
  rigidBody->setUserPointer(this);
  if (!isDynamic) {
    // Assume kinematic.
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags()
      | btCollisionObject::CF_KINEMATIC_OBJECT);
    rigidBody->setActivationState(DISABLE_DEACTIVATION);
  }
  physics.getDynamicsWorld()->addRigidBody(rigidBody);
}

void PhysicsCollider::didCollide(const PhysicsCollider& other) const {
  // TODO: have some default collision behavior.
  //std::cout << "BAM!\n";
}

void PhysicsCollider::update(const UpdateInfo& info) {
  std::shared_ptr<GameObject> gameObjectPtr = gameObject.lock();
  if (isDynamic) {
    btTransform newTransform;
    motionState->getWorldTransform(newTransform);
    btVector3 origin = newTransform.getOrigin();

    // TODO: potentially update rotation as well. We don't really need it now.
    gameObjectPtr->setWorldPosition(
     Ogre::Vector3(origin.x(), origin.y(), origin.z())
    );
  } else {
    btTransform oldTransform;
    motionState->getWorldTransform(oldTransform);
    Ogre::Vector3 cur = gameObjectPtr->getWorldPosition();
    oldTransform.setOrigin(btVector3(cur.x, cur.y, cur.z));
    motionState->setWorldTransform(oldTransform);
  }
}

void PhysicsCollider::applyWorldForce(const Ogre::Vector3& force) {
  rigidBody->applyCentralForce(btVector3(force.x, force.y, force.z));
}

void PhysicsCollider::applyWorldImpulse(const Ogre::Vector3& impulse) {
  rigidBody->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

