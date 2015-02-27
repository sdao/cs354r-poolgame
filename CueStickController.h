#pragma once

#include "Component.h"

class CueStickController : public Component {
public:
  CueStickController(std::weak_ptr<GameObject> go);
  virtual void update(const UpdateInfo& update) const override;
};

