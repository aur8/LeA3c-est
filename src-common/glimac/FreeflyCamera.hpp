#pragma once

#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"

class FreeflyCamera {
private:
    glm::vec3 m_position;
    float     m_phi;
    float     m_tetha;
    glm::vec3 m_LeftVector;
    glm::vec3 m_FrontVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors();

public:
    FreeflyCamera()
        : m_position(glm::vec3(0, 0, 0)), m_phi(glm::pi<float>()), m_tetha(0)
    {
        computeDirectionVectors();
    };

    void moveLeft(float t);
    void moveFront(float t);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
};