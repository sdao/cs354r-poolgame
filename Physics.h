#pragma once

#include <btBulletDynamicsCommon.h>
#include <OgreVector3.h>

// Forward declarations.
class PhysicsCollider;

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
  Physics(float gravity = 0.0);
  ~Physics();
  
  void stepSimulation(float timestep);
  btDynamicsWorld* getDynamicsWorld();
  PhysicsCollider* rayCast(const Ogre::Vector3& start,
                           const Ogre::Vector3& direction,
                           float maxDist) const;
  const std::vector<PhysicsCollider*>
  rayCastAll(const Ogre::Vector3& start,
             const Ogre::Vector3& direction,
             float maxDist) const;
};
