#include "Boid.hpp"
#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <vector>
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

void Boid::update_position(float delta_time, float ratio)
{
    // apply velocity to the position
    m_pos.x += m_vel.x * delta_time;
    m_pos.y += m_vel.y * delta_time;
    m_pos.z += m_vel.z * delta_time;

    // move position if boid not in world
    m_pos.x = stay_in_world(m_pos.x, ratio, -ratio);
    m_pos.y = stay_in_world(m_pos.y, 1.f, -1.f);
    m_pos.z = stay_in_world(m_pos.z, 0.f, -2.f);
}

void Boid::update_velocity()
{
    // add a speed factor to direction
    m_vel = glm::vec3(speed_factor) * glm::normalize(m_direction);
}

void Boid::update_direction(const std::vector<Boid>& boids)
{
    // apply all comportments to direction
    m_direction += glm::vec3(alignment_magnitude) * this->alignment(boids) + glm::vec3(cohesion_magnitude) * this->cohesion(boids) + glm::vec3(separation_magnitude) * this->separation(boids);

    m_direction = glm::normalize(m_direction);
}

// boids which go out world will reappear to the other side
float Boid::stay_in_world(const float& value, const float& max, const float& min)
{
    if (value >= max)
    {
        return min + std::fmod(value - min, max - min);
    }
    if (value <= min)
    {
        return max - std::fmod(max - value, max - min);
    }
    return value;
}

std::vector<Boid> Boid::get_neighbors(const std::vector<Boid>& boids, const float& distance_max)
{
    std::vector<Boid> neighbors{};
    for (const auto& other_boid : boids)
    {
        if (&other_boid != this)
        {
            if (glm::distance(other_boid.get_pos(), m_pos) <= distance_max)
            {
                neighbors.push_back(other_boid);
            }
        }
    }

    return neighbors;
}

glm::vec3 Boid::cohesion(const std::vector<Boid>& boids)
{
    // find the neighbors
    std::vector<Boid> neighbors = get_neighbors(boids, distance_max);

    // initialise our cohesion vector
    glm::vec3 coh(0.f);

    // verify if the boid have neighbors
    if (neighbors.empty())
    {
        return coh;
    }
    // apply cohesion to all neighbors
    for (auto other : neighbors)
    {
        coh += other.get_pos();
    }

    // divise by the number of neighbors
    coh /= static_cast<float>(neighbors.size());

    // obtain a final vector
    coh -= m_pos;

    return glm::normalize(coh);
}

glm::vec3 Boid::alignment(const std::vector<Boid>& boids)
{
    // find the neighbors
    std::vector<Boid> neighbors = get_neighbors(boids, distance_max);

    // initialise our alignment vector
    glm::vec3 ali(0.f);

    // verify if the boid have neighbors
    if (neighbors.empty())
    {
        return ali;
    }
    // apply cohesion to all neighbors
    for (auto other : neighbors)
    {
        ali += other.get_direction();
    }

    return glm::normalize(ali);
}

glm::vec3 Boid::separation(const std::vector<Boid>& boids)
{
    // find the neighbors
    std::vector<Boid> neighbors = get_neighbors(boids, distance_max);

    // initialise our separation vector
    glm::vec3 sep(0.f);

    // verify if the boid have neighbors
    if (neighbors.empty())
    {
        return sep;
    }

    for (auto other : neighbors)
    {
        glm::vec3 towardsMe;
        towardsMe = this->get_pos() - other.get_pos();

        if (length(towardsMe) > 0)
        {
            sep += glm::normalize(towardsMe) / length(towardsMe);
        }
    }
    return glm::normalize(sep);
}

void Boid::display()
{
    // à compléter
}