#pragma once

#include <btBulletDynamicsCommon.h>

class Physics {
  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;
  btDiscreteDynamicsWorld* dynamicsWorld;

  /**
   * Creates a new physics simulation system.
   * 
   * @param gravity acceleration due to gravity in the local coordinate system
   *                (by default, is 9.8 m/s^2, appropriate for metric units)
   */
  Physics(float gravity = 9.8);
  ~Physics();
  
  void stepSimulation(float timestep);
  btDynamicsWorld* getDynamicsWorld();
};
