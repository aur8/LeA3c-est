
#include "Boid.hpp"
#include "Character.hpp"
#include "GLFW/glfw3.h"
#include "Light.hpp"
#include "Model.hpp"
#include "Program.hpp"
#include "glimac/FreeflyCamera.hpp"
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
  /* load shader */
  Program program;

  /* Load texture */

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

  /* BOIDS */
  boids_model.create_vbo();
  boids_model.create_vao();

  /* Création character */
  Character character;

  character_model.create_vbo();
  character_model.create_vao();

  /* CUBE */
  cube_model.create_vbo();
  cube_model.create_vao();

  /* ENVIRONMENT */

  environment_model.create_vbo();
  environment_model.create_vao();

  /* BOIDS TAB */
  std::vector<Boid> boids(boid_number);

  /* LIGHT */

  Point_light light_gate(glm::vec3(0.f, 20.f, 0.f),
                         glm::vec3(100.f, 300.f, 500.f), program._program);
  Point_light light_character(character.get_pos() + glm::vec3(0.f, 5.f, 0.f),
                              glm::vec3(10.f, 0.f, 20.f), program._program);
  Dir_light ambient(glm::vec3(1.f, -1.f, 1.f), glm::vec3(1.f, 1.f, 1.f),
                    program._program);

  // Declare your infinite update loop.
  ctx.update = [&]() {
    // Clear the background with a fading effect
    ctx.use_stroke = false;
    ctx.background({0.2f, 0.1f, 0.3f});

    program._program.use();

    /* IMGUI */

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

    draw_point_light(light_gate, viewMatrix, 2);

    draw_dir_light(ambient, viewMatrix);

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

    light_character.m_position = character.get_pos() + glm::vec3(0.f, 5.f, 0.f);
    draw_point_light(light_character, viewMatrix, 1);

    camera.follow_character(character.get_pos());
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture_papier);
  glDeleteTextures(1, &texture_cube);
}