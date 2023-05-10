#pragma once

#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Character {
private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;

public:
    /* CONSTRUCTORS */
    Character()
        : m_position(0.), m_velocity(0.){};

    Character(glm::vec3 position)
        : m_position(position), m_velocity(0.){};

    /* MOVEMENTS */

    void move(glm::vec3 vector);
};
