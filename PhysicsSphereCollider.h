#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"
#include "PhysicsCollider.h"
#include <memory>

class PhysicsSphereCollider : public PhysicsCollider { 
public: 
  PhysicsSphereCollider(std::shared_ptr<GameObject> go, Physics& physics, float radius, float mass);
};
