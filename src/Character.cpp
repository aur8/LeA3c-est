#include "Character.hpp"
#include <climits>
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

void Character::move(float x, float y, float z, float tetha, float phi)
{
    glm::vec3 local_move(x, y, z);

    m_rotate_left += glm::radians(phi);
    m_rotate_up += glm::radians(tetha);

    glm::vec3 front_direction(
        std::cos(m_rotate_up) * std::sin(m_rotate_left),
        std::sin(m_rotate_up),
        std::cos(m_rotate_up) * std::cos(m_rotate_left)
    );

    glm::vec3 left_direction(
        std::sin(m_rotate_left + glm::pi<float>() / 2.f),
        0.f,
        std::cos(m_rotate_left + glm::pi<float>() / 2.f)
    );

    glm::vec3 up_direction = -glm::cross(front_direction, left_direction);

    glm::vec3 final_move = front_direction * local_move.z + left_direction * local_move.x + up_direction * local_move.y;

    m_position += final_move * m_speed;
}