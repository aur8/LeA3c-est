#include "Character.hpp"
#include "Boid.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <climits>

void Character::move(float x, float y, float z, float tetha, float phi) {
  glm::vec3 local_move(x, y, z);

  m_rotate_left += glm::radians(phi);
  m_rotate_up += glm::radians(tetha);

  glm::vec3 front_direction(std::cos(m_rotate_up) * std::sin(m_rotate_left),
                            std::sin(m_rotate_up),
                            std::cos(m_rotate_up) * std::cos(m_rotate_left));

  glm::vec3 left_direction(std::sin(m_rotate_left + glm::pi<float>() / 2.f),
                           0.f,
                           std::cos(m_rotate_left + glm::pi<float>() / 2.f));

  glm::vec3 up_direction = glm::cross(front_direction, left_direction);

  glm::vec3 final_move = front_direction * local_move.z +
                         left_direction * local_move.x +
                         up_direction * local_move.y;

  m_position += final_move * m_speed;
}

void handle_movement(Character &character, FreeflyCamera &camera,
                     bool &right_rot, bool &up_rot, bool &left_rot,
                     bool &down_rot, bool &left_move, bool &right_move,
                     bool &front_move, bool &back_move, p6::Context &ctx) {
  if (right_rot) {
    camera.rotateLeft(-1.f);
    character.move(0.f, 0.f, 0.f, 0.f, -1.f);
  }
  if (left_rot) {
    camera.rotateLeft(1.f);
    character.move(0.f, 0.f, 0.f, 0.f, 1.f);
  }
  if (up_rot) {
    camera.rotateUp(1.f);
    character.move(0.f, 0.f, 0.f, 1.f, 0.f);
  }
  if (down_rot) {
    camera.rotateUp(-1.f);
    character.move(0.f, 0.f, 0.f, -1.f, 0.f);
  }
  if (left_move) {
    // camera.moveLeft(0.5f);
    character.move(-2.5f, 0., 0., 0., 0.);
  }
  if (right_move) {
    // camera.moveLeft(-0.5f);
    character.move(2.5f, 0., 0., 0., 0.);
  }
  if (front_move) {
    // camera.moveFront(-0.5f);
    character.move(0.f, 0.f, -2.5f, 0.f, 0.f);
  }
  if (back_move) {
    // camera.moveFront(0.5f);
    character.move(0.f, 0.f, 2.5f, 0.f, 0.f);
  }

  ctx.key_pressed = [&right_rot, &up_rot, &left_rot, &down_rot, &left_move,
                     &right_move, &front_move, &back_move](p6::Key key) {
    if (key.physical == GLFW_KEY_D) {
      right_rot = true;
    }
    if (key.physical == GLFW_KEY_A) {
      left_rot = true;
    }
    if (key.physical == GLFW_KEY_W) {
      up_rot = true;
    }
    if (key.physical == GLFW_KEY_S) {
      down_rot = true;
    }
    if (key.physical == GLFW_KEY_LEFT) {
      left_move = true;
    }
    if (key.physical == GLFW_KEY_RIGHT) {
      right_move = true;
    }
    if (key.physical == GLFW_KEY_UP) {
      front_move = true;
    }
    if (key.physical == GLFW_KEY_DOWN) {
      back_move = true;
    }
  };

  ctx.key_released = [&right_rot, &up_rot, &left_rot, &down_rot, &left_move,
                      &right_move, &front_move, &back_move](p6::Key key) {
    if (key.physical == GLFW_KEY_D) {
      right_rot = false;
    }
    if (key.physical == GLFW_KEY_A) {
      left_rot = false;
    }
    if (key.physical == GLFW_KEY_W) {
      up_rot = false;
    }
    if (key.physical == GLFW_KEY_S) {
      down_rot = false;
    }
    if (key.physical == GLFW_KEY_LEFT) {
      left_move = false;
    }
    if (key.physical == GLFW_KEY_RIGHT) {
      right_move = false;
    }
    if (key.physical == GLFW_KEY_UP) {
      front_move = false;
    }
    if (key.physical == GLFW_KEY_DOWN) {
      back_move = false;
    }
  };

  ctx.mouse_dragged = [&camera](const p6::MouseDrag &button) {
    camera.rotateLeft(button.delta.x * 5);
    camera.rotateUp(-button.delta.y * 5);
  };

  ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
    camera.moveFront(-scroll.dy);
  };
}

void Character::draw_character(glm::mat4 viewMatrix, Program &program,
                               p6::Context &ctx, GLuint &texture,
                               Model &character_model) {

  glBindVertexArray(character_model.get_vao());

  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
  glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

  MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, 0.f});
  MVMatrix = glm::translate(MVMatrix, m_position);
  MVMatrix = glm::scale(MVMatrix, glm::vec3{0.5f});
  MVMatrix = viewMatrix * MVMatrix;

  glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE,
                     glm::value_ptr(ProjMatrix * MVMatrix));
  glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
  glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE,
                     glm::value_ptr(NormalMatrix));

  glDrawArrays(GL_TRIANGLES, 0, character_model.getVertices().size());
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
