#include "Boid.hpp"
#include "Character.hpp"
#include "GLFW/glfw3.h"
#include "Model.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include <iostream>
#include <vector>

static int boid_number = 100;

struct VertexCube {
  glm::vec3 m_position;
  glm::vec3 m_normal;
  glm::vec2 m_texCoords;

  VertexCube()
      : m_position(glm::vec3()), m_normal(glm::vec3()),
        m_texCoords(glm::vec2()) {}

  VertexCube(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords)
      : m_position(position), m_normal(normal), m_texCoords(texCoords) {}
};
int main() {
  // Actual app
  auto ctx = p6::Context{{.title = "Papeterie"}};
  //   ctx.maximize_window();

  FreeflyCamera camera;
  bool right_rot = false;
  bool left_rot = false;
  bool up_rot = false;
  bool down_rot = false;
  bool right_move = false;
  bool left_move = false;
  bool front_move = false;
  bool back_move = false;

  Params params = {};

  /*  LOADING MODELS */

  Model character_model = Model("kaonashi.obj");
  Model cube_model = Model("cube3.obj");
  Model boids_model = Model("paper.obj");
  Model environment_model = Model("environment2.obj");

  ///////////////////////////
  // boids 3D avec OPENGL //
  /////////////////////////

  // load shader
  const p6::Shader shader =
      p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

  // variable uniform
  GLint uMVPMatrix_location = glGetUniformLocation(shader.id(), "uMVPMatrix");
  GLint uMVMatrix_location = glGetUniformLocation(shader.id(), "uMVMatrix");
  GLint uNormalMatrix_location =
      glGetUniformLocation(shader.id(), "uNormalMatrix");

  glEnable(GL_DEPTH_TEST);

  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
  glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

  // VBO BOIDS
  GLuint vbo_boids = 0;
  glGenBuffers(1, &vbo_boids);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_boids);

  // Création boids
  // const std::vector<glimac::ShapeVertex> boids_vertices =
  //     glimac::sphere_vertices(1.f, 32, 16);

  const std::vector<glimac::ShapeVertex> boids_vertices =
      boids_model.getVertices();

  // envoie des données au GPU
  glBufferData(GL_ARRAY_BUFFER,
               boids_model.getVertices().size() * sizeof(glimac::ShapeVertex),
               boids_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // VAO BOIDS
  GLuint vao_boids = 0;
  glGenVertexArrays(1, &vao_boids);
  glBindVertexArray(vao_boids);

  // binding vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo_boids);

  //à garder
  const GLuint VERTEX_ATTR_POSITION = 0;
  const GLuint VERTEX_ATTR_NORMAL = 1;
  const GLuint VERTEX_ATTR_TEXCOORDS = 2;

  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

  glVertexAttribPointer(
      VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(glimac::ShapeVertex),
                        (const GLvoid *)offsetof(glimac::ShapeVertex, normal));
  glVertexAttribPointer(
      VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, texCoords));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // VBO CHARACTER
  GLuint vbo_character = 0;
  glGenBuffers(1, &vbo_character);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_character);

  // VAO CHARACTER
  GLuint vao_character = 0;
  glGenVertexArrays(1, &vao_character);
  glBindVertexArray(vao_character);

  // Création character
  Character character;

  const std::vector<glimac::ShapeVertex> character_vertices =
      character_model.getVertices();

  // envoie des données au GPU
  glBufferData(GL_ARRAY_BUFFER,
               character_model.getVertices().size() *
                   sizeof(glimac::ShapeVertex),
               character_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo_character);

  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

  glVertexAttribPointer(
      VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(glimac::ShapeVertex),
                        (const GLvoid *)offsetof(glimac::ShapeVertex, normal));
  glVertexAttribPointer(
      VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, texCoords));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // VBO CUBE
  GLuint vbo_cube = 0;
  glGenBuffers(1, &vbo_cube);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);

  // VAO CUBE
  GLuint vao_cube = 0;
  glGenVertexArrays(1, &vao_cube);
  glBindVertexArray(vao_cube);

  // CREATE CUBE

  const std::vector<glimac::ShapeVertex> cube_vertices =
      cube_model.getVertices();

  // envoie des données au GPU
  glBufferData(GL_ARRAY_BUFFER,
               cube_model.getVertices().size() * sizeof(glimac::ShapeVertex),
               cube_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);

  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

  glVertexAttribPointer(
      VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(glimac::ShapeVertex),
                        (const GLvoid *)offsetof(glimac::ShapeVertex, normal));
  glVertexAttribPointer(
      VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, texCoords));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // VBO ENVIRONMENT
  GLuint vbo_environment = 0;
  glGenBuffers(1, &vbo_environment);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_environment);

  // VAO ENVIRONMENT
  GLuint vao_environment = 0;
  glGenVertexArrays(1, &vao_environment);
  glBindVertexArray(vao_environment);

  // CREATE ENVIRONMENT

  const std::vector<glimac::ShapeVertex> environment_vertices =
      environment_model.getVertices();

  // envoie des données au GPU
  glBufferData(GL_ARRAY_BUFFER,
               environment_model.getVertices().size() *
                   sizeof(glimac::ShapeVertex),
               environment_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo_environment);

  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

  glVertexAttribPointer(
      VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(glimac::ShapeVertex),
                        (const GLvoid *)offsetof(glimac::ShapeVertex, normal));
  glVertexAttribPointer(
      VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, texCoords));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  std::vector<glm::vec3> positions;
  for (int i = 0; i < 32; ++i) {
    positions.push_back(glm::sphericalRand(2.0f));
  }

  /* BOIDS TAB */
  std::vector<Boid> boids(boid_number);

  // Declare your infinite update loop.
  ctx.update = [&]() {
    // Clear the background with a fading effect
    ctx.use_stroke = false;
    ctx.background({0.2f, 0.1f, 0.3f});

    ImGui::Begin("Test");
    ImGui::SliderFloat("Cohesion Magnitude", &params.cohesion_magnitude, 0.f,
                       1.f);
    ImGui::SliderFloat("Aligment Magnitude", &params.alignment_magnitude, 0.f,
                       1.f);
    ImGui::SliderFloat("Separation Magnitude", &params.separation_magnitude,
                       0.f, 1.f);
    ImGui::SliderFloat("Distance with neighbors", &params.distance_max, 0.f,
                       1.f);
    ImGui::End();

    // EVENEMENT CAMERA

    // camera
    if (right_rot) {
      camera.rotateLeft(-1.f);
    }
    if (left_rot) {
      camera.rotateLeft(1.f);
    }
    if (up_rot) {
      camera.rotateUp(1.f);
    }
    if (down_rot) {
      camera.rotateUp(-1.f);
    }
    if (left_move) {
      // camera.moveLeft(0.5f);
      character.move(-2.5f, 0., 0.);
    }
    if (right_move) {
      // camera.moveLeft(-0.5f);
      character.move(2.5f, 0., 0.);
    }
    if (front_move) {
      // camera.moveFront(-0.5f);
      character.move(0.f, 0.f, -2.5f);
    }
    if (back_move) {
      // camera.moveFront(0.5f);
      character.move(0.f, 0.f, 2.5f);
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

    glm::mat4 viewMatrix = camera.getViewMatrix();

    // for (auto &boid : boids) {
    //   boid.update_direction(boids);
    //   boid.update_velocity();
    //   boid.update_position(ctx.delta_time(), ctx.aspect_ratio());

    //   ctx.fill = {p6::random::number(0.5, 1), p6::random::number(0.5, 1),
    //               p6::random::number(0.5, 1)};
    //   ctx.circle(p6::Center{boid.get_pos().x, boid.get_pos().y},
    //              p6::Radius{0.01f});
    // }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glBindVertexArray(vao_boids);

    for (auto &boid : boids) {
      MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, 0.f}); // Translation
      //   MVMatrix = glm::rotate(MVMatrix, ctx.time(),
      //                          AxesRotation.at(i)); // Translation * Rotation
      MVMatrix = glm::translate(
          MVMatrix,
          boid.get_pos()); // Translation * Rotation * Translation
      MVMatrix = glm::scale(
          MVMatrix,
          glm::vec3{0.1f}); // Translation * Rotation * Translation * Scale
      MVMatrix = viewMatrix * MVMatrix;

      glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(ProjMatrix * MVMatrix));
      glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(MVMatrix));
      glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(NormalMatrix));

      glDrawArrays(GL_TRIANGLES, 0, boids_vertices.size());

      boid.update(ctx.delta_time(), 5, boids, params);
    }
    glBindVertexArray(0);

    glBindVertexArray(vao_character);
    MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, 0.f});
    MVMatrix = glm::translate(MVMatrix, character.get_pos());
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.5f});
    MVMatrix = viewMatrix * MVMatrix;

    glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE,
                       glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE,
                       glm::value_ptr(NormalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, character_vertices.size());

    glBindVertexArray(0);

    camera.follow_character(character.get_pos());

    glBindVertexArray(vao_cube);
    glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size());
    glBindVertexArray(0);

    glBindVertexArray(vao_environment);
    glDrawArrays(GL_TRIANGLES, 0, environment_vertices.size());
    glBindVertexArray(0);
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}