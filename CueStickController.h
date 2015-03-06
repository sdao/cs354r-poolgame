#pragma once

#include "Component.h"

class CueStickController : public Component {
  std::weak_ptr<GameObject> currentObj;

public:
  CueStickController(std::weak_ptr<GameObject> go);
  virtual void update(const UpdateInfo& update) override;

  /**
   * Hits with the cue stick, applying an impulse to the object right in front
   * of the cue stick's trajectory.
   * TODO: convert the strength parameter to an enum
   * 
   * @param strength the strength of the hit; "Low", "Medium", or "High"
   */
  void hit(std::string strength);
};

