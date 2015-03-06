#pragma once

#include "GameObject.h"

enum class BallType {
  BALL_POOL = 0,
  BALL_CUE = 1,
  BALL_GOAL = 2
};

class Ball : public GameObject {
  float radius;

  std::string createSphere(const std::string& strName,
               const float r,
               const int nRings = 16,
               const int nSegments = 32);

public:
  Ball(Ogre::SceneManager* mgr,
       const std::string& name,
       float radius,
       const Ogre::Vector3& position,
       BallType type = BallType::BALL_POOL);
};

