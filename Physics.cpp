#include "Physics.h"

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
}

btDynamicsWorld* Physics::getDynamicsWorld() {
  return dynamicsWorld;
}

