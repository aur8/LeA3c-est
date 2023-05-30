#pragma once

#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Character {
private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float     m_speed;
    float     m_rotate_up;
    float     m_rotate_left;

public:
    /* CONSTRUCTORS */
    Character()
        : m_position(0., 0., -2.f), m_velocity(0.), m_speed(0.2f), m_rotate_up(0.f), m_rotate_left(0.f){};

    Character(glm::vec3 position)
        : m_position(position), m_velocity(0.), m_speed(0.2f), m_rotate_up(0.f), m_rotate_left(0.f){};

    /* GETTER */
    glm::vec3 get_pos() { return m_position; }

    /* MOVEMENTS */

    void move(float x, float y, float z, float teta, float phi);
};
