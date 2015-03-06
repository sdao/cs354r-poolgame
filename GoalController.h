#pragma once

#include "Component.h"
#include "GameInfo.h"

class GoalController : public Component {
	std::shared_ptr<GameInfo> gameinfo;

public:
  GoalController(std::shared_ptr<GameInfo>);
  virtual void didCollide(PhysicsCollider& collider) override;
};

