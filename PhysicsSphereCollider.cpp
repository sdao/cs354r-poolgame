#include "PhysicsSphereCollider.h"

#include <iostream>

PhysicsSphereCollider::PhysicsSphereCollider(GameObject* go,
                                             Physics& physics,
                                             float radius,
                                             float mass)
  : gameObject(go)
{
  collisionShape = new btSphereShape(btScalar(radius));
  
  btVector3 localInertia(0, 0, 0);
  collisionShape->calculateLocalInertia(mass, localInertia);
  
  Ogre::Vector3 objPosition = gameObject->getPosition();
  btTransform startTransform;
  startTransform.setIdentity();
  startTransform.setOrigin(btVector3(objPosition.x, objPosition.y, objPosition.z));
    
  motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
                                                  motionState,
                                                  collisionShape,
                                                  localInertia);
  rigidBody = new btRigidBody(rbInfo);
  rigidBody->setUserPointer(this);
  physics.getDynamicsWorld()->addRigidBody(rigidBody);
}

PhysicsSphereCollider::~PhysicsSphereCollider() {
  delete rigidBody;
  delete motionState;
  delete collisionShape;
}

void PhysicsSphereCollider::update() const {
  std::cout << "updating\n";
  btTransform newTransform;
  motionState->getWorldTransform(newTransform);
  btVector3 origin = newTransform.getOrigin();
  // TODO: potentially update rotation as well. We don't really need it now.
  gameObject->setPosition(Ogre::Vector3(origin.x(), origin.y(), origin.z()));
}

