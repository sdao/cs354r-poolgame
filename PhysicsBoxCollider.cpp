#include "PhysicsBoxCollider.h"

PhysicsBoxCollider::PhysicsBoxCollider(GameObject* go,
                                             Physics& physics,
                                             const Ogre::Vector3& dim,
                                             float mass)
  : gameObject(go)
{
  collisionShape =
    new btBoxShape(btVector3(dim.x * 0.5f, dim.y * 0.5f, dim.z * 0.5f));
  
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

PhysicsBoxCollider::~PhysicsBoxCollider() {
  delete rigidBody;
  delete motionState;
  delete collisionShape;
}

void PhysicsBoxCollider::update() const {
  btTransform newTransform;
  motionState->getWorldTransform(newTransform);
  btVector3 origin = newTransform.getOrigin();
  // TODO: potentially update rotation as well. We don't really need it now.
  gameObject->setPosition(Ogre::Vector3(origin.x(), origin.y(), origin.z()));
}

