#include "Boid.hpp"
#include "glimac/common.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"
#include <glimac/sphere_vertices.hpp>
#include <iostream>

// initialisation magnitude

float Boid::cohesion_magnitude = 0.5f;
float Boid::alignment_magnitude = 0.5f;
float Boid::separation_magnitude = 0.5f;
float Boid::distance_max = 0.5f;

int main() {
  // { // Run the tests
  //     if (doctest::Context{}.run() != 0)
  //         return EXIT_FAILURE;
  //     // The CI does not have a GPU so it cannot run the rest of the code.
  //     const bool no_gpu_available =
  //         argc >= 2 && strcmp(argv[1], "-nogpu") == 0; //
  //         NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  //     if (no_gpu_available)
  //         return EXIT_SUCCESS;
  // }

  // Actual app
  auto ctx = p6::Context{{.title = "Papeterie"}};
  //   ctx.maximize_window();

  std::vector<Boid> boids(100);

  // initialisation des positions de boid
  for (auto &boid : boids) {
    boid.set_pos(
        glm::vec2(p6::random::number(-2, 2), p6::random::number(-1, 1)));
  }
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

  // Declare your infinite update loop.
  ctx.update = [&]() {
    // Clear the background with a fading effect
    ctx.use_stroke = false;
    ctx.fill = {0.2f, 0.1f, 0.3f, 0.1f};
    // ctx.fill = {p6::random::number(-1, 0.5), p6::random::number(-1, 0.5),
    //             p6::random::number(-1, 0.5)}; // random
    // ctx.rectangle(p6::FullScreen{});

    ctx.background({0.2f, 0.1f, 0.3f});
    // ctx.fill = {1.f, 0.7f, 0.2f};
    // ctx.fill = {p6::random::number(0.5, 1), p6::random::number(0.5, 1),
    //             p6::random::number(0.5, 1)}; // random

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

    for (size_t i = 1; i < 32; i++) {

      MVMatrix =
          glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f}); // Translation
      //   MVMatrix = glm::rotate(MVMatrix, ctx.time(),
      //                          AxesRotation.at(i)); // Translation * Rotation
      MVMatrix = glm::translate(
          MVMatrix,
          positions.at(i)); // Translation * Rotation * Translation
      MVMatrix = glm::scale(
          MVMatrix,
          glm::vec3{0.2f}); // Translation * Rotation * Translation * Scale

      glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(ProjMatrix * MVMatrix));
      glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(MVMatrix));
      glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(NormalMatrix));

      glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    glBindVertexArray(0);

    glBindVertexArray(0);
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}