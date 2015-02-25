#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

class PhysicsCollider : public Component {
public:  
  virtual ~PhysicsCollider();
  virtual void update(float deltaTime) const = 0;
  virtual void didCollide(const PhysicsCollider& other) const;

};
