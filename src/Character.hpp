#pragma once

#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Character {
private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float     m_speed;

public:
    /* CONSTRUCTORS */
    Character()
        : m_position(0., 0., -5.f), m_velocity(0.), m_speed(0.2f){};

    Character(glm::vec3 position)
        : m_position(position), m_velocity(0.), m_speed(0.2f){};

    /* GETTER */
    glm::vec3 get_pos() { return m_position; }

    /* MOVEMENTS */

    void move(float x, float y, float z);
};
