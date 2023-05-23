#include <iostream>
#include <vector>
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

static int boid_number = 100;

int main()
{
    // Actual app
    auto ctx = p6::Context{{.title = "Papeterie"}};
    //   ctx.maximize_window();

    FreeflyCamera camera;
    bool          right_rot  = false;
    bool          left_rot   = false;
    bool          up_rot     = false;
    bool          down_rot   = false;
    bool          right_move = false;
    bool          left_move  = false;
    bool          front_move = false;
    bool          back_move  = false;

    Params params = {};

    /*  LOADING MODELS */

    Model character_model   = Model("kaonashi.obj");
    Model boids_model       = Model("paper.obj");
    Model cube_model        = Model("cube3.obj");
    Model environment_model = Model("environment2.obj");

    ///////////////////////////
    // boids 3D avec OPENGL //
    /////////////////////////

    // load shader
    const p6::Shader shader =
        p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    // variable uniform
    GLint uMVPMatrix_location = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrix_location  = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrix_location =
        glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix =
        glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    glm::mat4 MVMatrix     = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
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

    std::vector<glm::vec3> positions;
    for (int i = 0; i < 32; ++i)
    {
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
        ImGui::SliderFloat("Cohesion Magnitude", &params.cohesion_magnitude, 0.f, 1.f);
        ImGui::SliderFloat("Aligment Magnitude", &params.alignment_magnitude, 0.f, 1.f);
        ImGui::SliderFloat("Separation Magnitude", &params.separation_magnitude, 0.f, 1.f);
        ImGui::SliderFloat("Distance with neighbors", &params.distance_max, 0.f, 1.f);
        ImGui::End();

        // EVENEMENT CAMERA

        // camera
        if (right_rot)
        {
            camera.rotateLeft(-1.f);
        }
        if (left_rot)
        {
            camera.rotateLeft(1.f);
        }
        if (up_rot)
        {
            camera.rotateUp(1.f);
        }
        if (down_rot)
        {
            camera.rotateUp(-1.f);
        }
        if (left_move)
        {
            // camera.moveLeft(0.5f);
            character.move(-2.5f, 0., 0.);
        }
        if (right_move)
        {
            // camera.moveLeft(-0.5f);
            character.move(2.5f, 0., 0.);
        }
        if (front_move)
        {
            // camera.moveFront(-0.5f);
            character.move(0.f, 0.f, -2.5f);
        }
        if (back_move)
        {
            // camera.moveFront(0.5f);
            character.move(0.f, 0.f, 2.5f);
        }

        ctx.key_pressed = [&right_rot, &up_rot, &left_rot, &down_rot, &left_move,
                           &right_move, &front_move, &back_move](p6::Key key) {
            if (key.physical == GLFW_KEY_D)
            {
                right_rot = true;
            }
            if (key.physical == GLFW_KEY_A)
            {
                left_rot = true;
            }
            if (key.physical == GLFW_KEY_W)
            {
                up_rot = true;
            }
            if (key.physical == GLFW_KEY_S)
            {
                down_rot = true;
            }
            if (key.physical == GLFW_KEY_LEFT)
            {
                left_move = true;
            }
            if (key.physical == GLFW_KEY_RIGHT)
            {
                right_move = true;
            }
            if (key.physical == GLFW_KEY_UP)
            {
                front_move = true;
            }
            if (key.physical == GLFW_KEY_DOWN)
            {
                back_move = true;
            }
        };

        ctx.key_released = [&right_rot, &up_rot, &left_rot, &down_rot, &left_move,
                            &right_move, &front_move, &back_move](p6::Key key) {
            if (key.physical == GLFW_KEY_D)
            {
                right_rot = false;
            }
            if (key.physical == GLFW_KEY_A)
            {
                left_rot = false;
            }
            if (key.physical == GLFW_KEY_W)
            {
                up_rot = false;
            }
            if (key.physical == GLFW_KEY_S)
            {
                down_rot = false;
            }
            if (key.physical == GLFW_KEY_LEFT)
            {
                left_move = false;
            }
            if (key.physical == GLFW_KEY_RIGHT)
            {
                right_move = false;
            }
            if (key.physical == GLFW_KEY_UP)
            {
                front_move = false;
            }
            if (key.physical == GLFW_KEY_DOWN)
            {
                back_move = false;
            }
        };

        ctx.mouse_dragged = [&camera](const p6::MouseDrag& button) {
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
        glBindVertexArray(boids_model.get_vao());

        for (auto& boid : boids)
        {
            MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, 0.f}); // Translation
            //   MVMatrix = glm::rotate(MVMatrix, ctx.time(),
            //                          AxesRotation.at(i)); // Translation * Rotation
            MVMatrix = glm::translate(
                MVMatrix,
                boid.get_pos()
            ); // Translation * Rotation * Translation
            MVMatrix = glm::scale(
                MVMatrix,
                glm::vec3{0.1f}
            ); // Translation * Rotation * Translation * Scale
            MVMatrix = viewMatrix * MVMatrix;

            glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, boids_model.getVertices().size());

            boid.update(ctx.delta_time(), 5, boids, params);
        }
        glBindVertexArray(0);

        glBindVertexArray(character_model.get_vao());
        MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, 0.f});
        MVMatrix = glm::translate(MVMatrix, character.get_pos());
        MVMatrix = glm::scale(MVMatrix, glm::vec3{0.5f});
        MVMatrix = viewMatrix * MVMatrix;

        glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, character_model.getVertices().size());

        glBindVertexArray(0);

        camera.follow_character(character.get_pos());

        glBindVertexArray(cube_model.get_vao());
        glDrawArrays(GL_TRIANGLES, 0, cube_model.getVertices().size());
        glBindVertexArray(0);

        glBindVertexArray(environment_model.get_vao());
        glDrawArrays(GL_TRIANGLES, 0, environment_model.getVertices().size());
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}