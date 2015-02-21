#include "PhysicsSphereCollider.h"

PhysicsSphereCollider::PhysicsSphereCollider(GameObject& go,
                                             Physics& physics,
                                             float radius,
                                             float mass)
{
  collisionShape = new btSphereShape(btScalar(radius));
  
  btVector3 localInertia(0, 0, 0);
  collisionShape->calculateLocalInertia(mass, localInertia);
    
  btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 0, 0)); // TODO: get from gameobject transform.
    
  motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
	                                                motionState,
	                                                collisionShape,
	                                                localInertia);
	rigidBody = new btRigidBody(rbInfo);
  physics.getDynamicsWorld()->addRigidBody(rigidBody);
}

PhysicsSphereCollider::~PhysicsSphereCollider() {
  delete rigidBody;
  delete motionState;
  delete collisionShape;
}

void PhysicsSphereCollider::update() const {
  // TODO: set gameobject transform using the motionstate.
}
