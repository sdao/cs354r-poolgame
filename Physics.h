#pragma once

#include <btBulletDynamicsCommon.h>

class Physics {
  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;
  btDiscreteDynamicsWorld* dynamicsWorld;

public:
  /**
   * Creates a new physics simulation system.
   * 
   * @param gravity acceleration due to gravity in the local coordinate system
   *                (by default, is 980 cm/s^2, appropriate for metric units)
   */
  Physics(float gravity = 980.0);
  ~Physics();
  
  void stepSimulation(float timestep);
  btDynamicsWorld* getDynamicsWorld();
};
