#include "Character.hpp"
#include "glm/fwd.hpp"

void Character::move(float x, float y, float z)
{
    m_position += glm::vec3(m_speed * x, m_speed * y, m_speed * z);
}