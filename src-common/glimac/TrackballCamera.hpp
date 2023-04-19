#pragma once
#include "glm/fwd.hpp"

class TrackballCamera {
private:
  float m_distance;
  float m_AngleX;
  float m_AngleY;

public:
  explicit TrackballCamera(float Distance = 5.f, float AngleX = 0,
                           float AngleY = 0);

  // methods

  void moveFront(float delta);
  void rotateLeft(float degrees);
  void rotateUp(float degrees);

  glm::mat4 getViewMatrix() const;
};