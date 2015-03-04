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
                                 Physics& phys,
                                 float mass,
                                 float friction,
                                 bool trigger)
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
  rbInfo.m_rollingFriction = friction;
  rbInfo.m_linearDamping = 0.3f;
  rbInfo.m_angularDamping = 0.3f;
  rbInfo.m_restitution = 0.5f;
  rigidBody = new btRigidBody(rbInfo);
  rigidBody->setUserPointer(this);
  if (!isDynamic) {
    // Assume kinematic.
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags()
      | btCollisionObject::CF_KINEMATIC_OBJECT);
    rigidBody->setActivationState(DISABLE_DEACTIVATION);
  }
  if (trigger) {
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags()
      | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  }

  addToPhysics(phys);
}

void PhysicsCollider::reportCollision(PhysicsCollider& other) const {
  std::shared_ptr<GameObject> gameObjectPtr = gameObject.lock();
  gameObjectPtr->didCollide(other);
  //std::cout << "---ouch---\n";
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
  rigidBody->activate();
  rigidBody->applyCentralForce(btVector3(force.x, force.y, force.z));
}

void PhysicsCollider::applyWorldImpulse(const Ogre::Vector3& impulse) {
  rigidBody->activate();
  rigidBody->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

void PhysicsCollider::removeFromPhysics() {
  if (physics) {
    physics->getDynamicsWorld()->removeRigidBody(rigidBody);
    physics = nullptr;
  }
}

void PhysicsCollider::addToPhysics(Physics& phys) {
  phys.getDynamicsWorld()->addRigidBody(rigidBody);
  physics = &phys;
  //rigidBody->setGravity(btVector3(0, 0, 0));
}

