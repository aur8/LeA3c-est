#include "Boid.hpp"
#include "glimac/TrackballCamera.hpp"
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

// initialisation magnitude

float Boid::cohesion_magnitude = 0.5f;
float Boid::alignment_magnitude = 0.5f;
float Boid::separation_magnitude = 0.5f;
float Boid::distance_max = 0.5f;
float Boid::speed_factor = 0.5f;
static int boid_number = 100;

int main() {

  // Actual app
  auto ctx = p6::Context{{.title = "Papeterie"}};
  //   ctx.maximize_window();

  TrackballCamera camera;
  bool right = false;
  bool left = false;
  bool up = false;
  bool down = false;

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

  // VBO
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Création boids
  const std::vector<glimac::ShapeVertex> vertices =
      glimac::sphere_vertices(1.f, 32, 16);

  // envoie des données au GPU
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // VAO
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  const GLuint VERTEX_ATTR_POSITION = 0;
  const GLuint VERTEX_ATTR_NORMAL = 1;
  const GLuint VERTEX_ATTR_TEXCOORDS = 2;

  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

  // binding vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

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

  //   std::vector<glm::vec3> AxesRotation;
  std::vector<glm::vec3> positions;
  for (int i = 0; i < 32; ++i) {
    // AxesRotation.push_back(glm::sphericalRand(1.0f));
    positions.push_back(glm::sphericalRand(2.0f));
  }

  std::vector<Boid> boids(boid_number);
  // initialisation des positions de boid
  for (auto &boid : boids) {
    boid.set_pos(glm::vec3(p6::random::number(-2, 2), p6::random::number(-1, 1),
                           p6::random::number(-2, 0)));
  }

  // Declare your infinite update loop.
  ctx.update = [&]() {
    // Clear the background with a fading effect
    ctx.use_stroke = false;
    ctx.background({0.2f, 0.1f, 0.3f});

    ImGui::Begin("Test");
    ImGui::SliderFloat("Cohesion Magnitude", &Boid::cohesion_magnitude, 0.f,
                       1.f);
    ImGui::SliderFloat("Aligment Magnitude", &Boid::alignment_magnitude, 0.f,
                       1.f);
    ImGui::SliderFloat("Separation Magnitude", &Boid::separation_magnitude, 0.f,
                       1.f);
    ImGui::SliderFloat("Distance with neighbors", &Boid::distance_max, 0.f,
                       1.f);
    ImGui::End();

    // EVENEMENT CAMERA

    // camera
    if (right) {
      camera.rotateLeft(-1.f);
    }
    if (left) {
      camera.rotateLeft(1.f);
    }
    if (up) {
      camera.rotateUp(1.f);
    }
    if (down) {
      camera.rotateUp(-1.f);
    }

    ctx.key_pressed = [&right, &up, &left, &down](p6::Key key) {
      if (key.physical == GLFW_KEY_D) {
        right = true;
      }
      if (key.physical == GLFW_KEY_A) {
        left = true;
      }
      if (key.physical == GLFW_KEY_W) {
        up = true;
      }
      if (key.physical == GLFW_KEY_S) {
        down = true;
      }
    };

    ctx.key_released = [&right, &up, &left, &down](p6::Key key) {
      if (key.physical == GLFW_KEY_D) {
        right = false;
      }
      if (key.physical == GLFW_KEY_A) {
        left = false;
      }
      if (key.physical == GLFW_KEY_W) {
        up = false;
      }
      if (key.physical == GLFW_KEY_S) {
        down = false;
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
    glBindVertexArray(vao);

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

      glDrawArrays(GL_TRIANGLES, 0, vertices.size());

      boid.update_direction(boids);
      boid.update_velocity();
      boid.update_position(ctx.delta_time(), ctx.aspect_ratio());
    }

    glBindVertexArray(0);
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}