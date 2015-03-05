#pragma once

#include "Component.h"

class CueStickController : public Component {
  std::weak_ptr<GameObject> currentObj;

public:
  CueStickController(std::weak_ptr<GameObject> go);
  virtual void update(const UpdateInfo& update) override;
  void hit(std::string strength);
};

