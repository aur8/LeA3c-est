#pragma once

#include "Model.hpp"
#include "Program.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <vector>

class Character {
private:
  glm::vec3 m_position;
  glm::vec3 m_velocity;
  float m_speed;
  float m_rotate_up;
  float m_rotate_left;

public:
  /* CONSTRUCTORS */
  Character()
      : m_position(0., 0., -2.f), m_velocity(0.), m_speed(0.2f),
        m_rotate_up(0.f), m_rotate_left(0.f){};

  Character(glm::vec3 position)
      : m_position(position), m_velocity(0.), m_speed(0.2f), m_rotate_up(0.f),
        m_rotate_left(0.f){};

  /* GETTER */
  glm::vec3 get_pos() { return m_position; }

  /* MOVEMENTS */

  void move(float x, float y, float z, float teta, float phi);

  /* DRAW */
  void draw_character(glm::mat4 viewMatrix, Program &program, p6::Context &ctx,
                      GLuint &texture, Model &character_model);
};

void handle_movement(Character &character, FreeflyCamera &camera,
                     bool &right_rot, bool &up_rot, bool &left_rot,
                     bool &down_rot, bool &left_move, bool &right_move,
                     bool &front_move, bool &back_move, p6::Context &ctx);
