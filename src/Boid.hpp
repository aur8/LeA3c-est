
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Boid {
private:
    // attributes
    glm::vec3 m_pos;       // position
    glm::vec3 m_vel;       // velocity
    glm::vec3 m_direction; // direction

public:
    static float cohesion_magnitude;
    static float alignment_magnitude;
    static float separation_magnitude;
    static float distance_max;
    static float speed_factor;

    /* CONSTRUCTORS */
    Boid()
        : m_pos(0), m_vel(0), m_direction(p6::random::direction(), 0){};
    Boid(glm::vec3 position)
        : m_pos(position), m_vel(0), m_direction(p6::random::direction(), 0){};

    /* DESTRUCTOR */
    ~Boid() = default;

    /* GETTEUR */
    glm::vec3 get_pos() const { return m_pos; }
    glm::vec3 get_vel() const { return m_vel; }
    glm::vec3 get_direction() const { return m_direction; }

    /* SETTEUR */
    void set_pos(const glm::vec3 position) { m_pos = position; }

    /* DISPLAY */
    void display();

    /* BOID VELOCITY */

    void update_position(const float delta_time, const float ratio);
    void update_velocity();
    void update_direction(const std::vector<Boid>& boids);
    // void update_acceleration();

    /* CALCULS */

    /* NEIGHBORS */

    std::vector<Boid> get_neighbors(const std::vector<Boid>& boids, const float& distance_max);

    /* BOID RULES */

    // Cohesion
    glm::vec3 cohesion(const std::vector<Boid>& boids);

    // Separation
    glm::vec3 separation(const std::vector<Boid>& boids);

    // Alignment
    glm::vec3 alignment(const std::vector<Boid>& boids);

    /* BOID WITH WINDOW */
    float stay_in_world(const float& value, const float& max, const float& min);
};