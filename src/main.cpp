
#include "Boid.hpp"
#include "Character.hpp"
#include "GLFW/glfw3.h"
#include "Model.hpp"
#include "Program.hpp"
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

static int boid_number = 200;

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
  // load shader
  Program program;

  GLuint texture_papier = create_texture("assets/textures/paper.jpg");
  GLuint texture_cube = create_texture("assets/textures/daysky.jpg");

  /*  LOADING MODELS */

  Model character_model = Model("kaonashi.obj");
  Model boids_model = Model("paper.obj");
  Model cube_model = Model("cube3.obj");
  Model environment_model = Model("environment2.obj");

  ///////////////////////////
  // boids 3D avec OPENGL //
  /////////////////////////

  glEnable(GL_DEPTH_TEST);

  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
  glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

  // BOIDS
  boids_model.create_vbo();
  boids_model.create_vao();

  // Création character
  Character character;

  character_model.create_vbo();
  character_model.create_vao();

  // CUBE
  cube_model.create_vbo();
  cube_model.create_vao();

  // ENVIRONMENT

  environment_model.create_vbo();
  environment_model.create_vao();

  /* BOIDS TAB */
  std::vector<Boid> boids(boid_number);

  // Declare your infinite update loop.
  ctx.update = [&]() {
    // Clear the background with a fading effect
    ctx.use_stroke = false;
    ctx.background({0.2f, 0.1f, 0.3f});

    program._program.use();

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

    // EVENEMENT CAMERA & ARPENTEUR
    handle_movement(character, camera, right_rot, up_rot, left_rot, down_rot,
                    left_move, right_move, front_move, back_move, ctx);

    glm::mat4 viewMatrix = camera.getViewMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIXED LIGHT

    program._program.use();

    glm::vec3 lightPosition = glm::vec3(0.f, 20.f, 0.f);
    glm::vec3 lightViewPosition =
        glm::vec3(viewMatrix * glm::vec4(lightPosition, 1.f));
    glUniform3fv(program.uLightPosition2, 1, glm::value_ptr(lightViewPosition));

    glm::vec3 intensity = glm::vec3(100.f, 300.f, 500.f);
    glUniform3fv(program.uLightIntensity2, 1, glm::value_ptr(intensity));

    glm::vec3 lightDir = glm::vec3(1.f, -1.f, 1.f);
    glm::vec3 lightViewDir = glm::vec3(viewMatrix * glm::vec4(lightDir, 1.f));
    glUniform3fv(program.uLightDir, 1, glm::value_ptr(lightViewDir));

    intensity = glm::vec3(1.f, 1.f, 1.f);
    glUniform3fv(program.uLightIntensity3, 1, glm::value_ptr(intensity));

    glUniform3fv(program.uKd, 1, glm::value_ptr(glm::vec3(1.5f, 0.5f, 0.3f)));
    glUniform3fv(program.uKs, 1, glm::value_ptr(glm::vec3(1.5f, 0.9f, 0.6f)));
    glUniform1f(program.uShininess, 0.9f);

    // ENVIRONMENT

    environment_model.draw_model(viewMatrix, program, ctx, texture_papier);

    // CUBE

    cube_model.draw_model(viewMatrix, program, ctx, texture_cube);

    // BOIDS

    for (auto &boid : boids) {
      boid.draw_boid(viewMatrix, program, ctx, texture_papier, boids_model);
      boid.update(ctx.delta_time(), 10, boids, params);
    }

    // CHARACTER

    character.draw_character(viewMatrix, program, ctx, texture_papier,
                             character_model);

    lightPosition = character.get_pos() + glm::vec3(0.f, 5.f, 0.f);
    lightViewPosition = glm::vec3(viewMatrix * glm::vec4(lightPosition, 1.f));
    glUniform3fv(program.uLightPosition, 1, glm::value_ptr(lightViewPosition));

    // glm::vec3 tViewPoint  = viewCamera *
    // (glm::vec4(character.get_pos(), 1.0f) * glm::vec4(0.f, 1.f, -0.5f, 0.f));
    // glm::vec3 tLightPoint = glm::vec3(tViewPoint.x, tViewPoint.y,
    // tViewPoint.z); glUniform3fv(program.uLightPosition2, 1,
    // glm::value_ptr(tLightPoint));

    intensity = glm::vec3(10.f, 0.f, 20.f);
    glUniform3fv(program.uLightIntensity, 1, glm::value_ptr(intensity));

    camera.follow_character(character.get_pos());
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture_papier);
  glDeleteTextures(1, &texture_cube);
}