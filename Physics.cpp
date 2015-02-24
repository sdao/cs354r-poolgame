#include "Physics.h"
#include "PhysicsCollider.h"

Physics::Physics(float gravity)
  : broadphase(new btDbvtBroadphase()),
    collisionConfiguration(new btDefaultCollisionConfiguration()),
    dispatcher(new btCollisionDispatcher(collisionConfiguration)),
    solver(new btSequentialImpulseConstraintSolver()),
    dynamicsWorld(new btDiscreteDynamicsWorld(
      dispatcher, broadphase, solver, collisionConfiguration
    ))
{ 
  dynamicsWorld->setGravity(btVector3(0, -gravity, 0));
}

Physics::~Physics() {
  delete dynamicsWorld;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;
}

void Physics::stepSimulation(float timestep) {
  dynamicsWorld->stepSimulation(timestep, 10);
  
  // Handle collisions.
  int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

        PhysicsCollider* colliderA = static_cast<PhysicsCollider*>(obA->getUserPointer());
        PhysicsCollider* colliderB = static_cast<PhysicsCollider*>(obB->getUserPointer());

        colliderA->didCollide(*colliderB);
        colliderB->didCollide(*colliderA);
    }
}

btDynamicsWorld* Physics::getDynamicsWorld() {
  return dynamicsWorld;
}

