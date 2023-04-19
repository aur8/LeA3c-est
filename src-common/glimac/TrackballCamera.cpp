#include "TrackballCamera.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"

TrackballCamera::TrackballCamera(float Distance, float AngleX, float AngleY)
    : m_distance(Distance), m_AngleX(AngleX), m_AngleY(AngleY) {}

void TrackballCamera::moveFront(float delta) { m_distance -= delta; }

void TrackballCamera::rotateLeft(float degrees) { m_AngleY += degrees; }

void TrackballCamera::rotateUp(float degrees) { m_AngleX += degrees; }

glm::mat4 TrackballCamera::getViewMatrix() const {
  glm::mat4 matrix =
      glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -m_distance));
  matrix = glm::rotate(matrix, glm::radians(m_AngleX), glm::vec3(1, 0, 0));
  matrix = glm::rotate(matrix, glm::radians(m_AngleY), glm::vec3(0, 1, 0));

  return matrix;
}