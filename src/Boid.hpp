
#include "Model.hpp"
#include "Program.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include <vector>

#pragma once

struct Params {
  float cohesion_magnitude{0.5f};
  float alignment_magnitude{0.5f};
  float separation_magnitude{0.5f};
  float distance_max{0.5f};
  float speed_factor{0.5f};
};

class Boid {
private:
  // attributes
  glm::vec3 m_pos; // position
  glm::vec3 get_vel(float speed_factor) {
    return glm::vec3(speed_factor) * m_direction;
  };
  glm::vec3 m_direction; // direction

public:
  /* CONSTRUCTORS */
  Boid() : m_direction(p6::random::direction(), 0) {
    m_pos = glm::vec3(p6::random::number(-2, 2), p6::random::number(-1, 1),
                      p6::random::number(-2, 0));
  };
  Boid(glm::vec3 position)
      : m_pos(position), m_direction(p6::random::direction(), 0){};

  /* GETTEUR */
  glm::vec3 get_pos() const { return m_pos; }
  glm::vec3 get_direction() const { return m_direction; }

  /* SETTEUR */
  void set_pos(const glm::vec3 position) { m_pos = position; }

  /* BOID VELOCITY */

  void update(const float delta_time, const float ratio,
              const std::vector<Boid> &boids, Params &params);
  void update_position(const float delta_time, const float ratio,
                       const float speed_factor);
  void update_direction(const std::vector<Boid> &boids, const Params &params);

  /* NEIGHBORS */

  std::vector<Boid> get_neighbors(const std::vector<Boid> &boids,
                                  const float &distance_max);

  /* BOID RULES */

  // Cohesion
  glm::vec3 cohesion(const std::vector<Boid> &neighbors);

  // Separation
  glm::vec3 separation(const std::vector<Boid> &neighbors);

  // Alignment
  glm::vec3 alignment(const std::vector<Boid> &neighbors);

  // Draw
  void draw_boid(glm::mat4 viewMatrix, Program &program, p6::Context &ctx,
                 GLuint &texture, Model &boid_model);
};

/* WORLD */
float stay_in_world(const float &value, const float &max, const float &min);
