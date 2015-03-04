#pragma once

#include "Component.h"

class GoalController : public Component {
public:
  virtual void didCollide(PhysicsCollider& collider) override;
};

