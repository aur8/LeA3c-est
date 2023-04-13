#include <stdlib.h>
#include <cstdlib>
#include <string>
#include "Boid.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

// initialisation magnitude

float      Boid::cohesion_magnitude   = 0.5f;
float      Boid::alignment_magnitude  = 0.5f;
float      Boid::separation_magnitude = 0.5f;
float      Boid::distance_max         = 0.5f;
float      Boid::speed_factor         = 0.1f;
static int boid_number                = 200;

int main(int argc, char* argv[])
{
    // Actual app
    auto ctx = p6::Context{{.title = "ProgS4"}};
    ctx.maximize_window();

    std::vector<Boid> boids(boid_number);

    // initialisation des positions de boid
    for (auto& boid : boids)
    {
        boid.set_pos(
            glm::vec2(p6::random::number(-2, 2), p6::random::number(-1, 1))
        );
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Clear the background with a fading effect
        ctx.use_stroke = false;
        ctx.background({0.2f, 0.1f, 0.3f});

        ImGui::Begin("Test");
        ImGui::SliderFloat("Cohesion Magnitude", &Boid::cohesion_magnitude, 0.f, 1.f);
        ImGui::SliderFloat("Aligment Magnitude", &Boid::alignment_magnitude, 0.f, 1.f);
        ImGui::SliderFloat("Separation Magnitude", &Boid::separation_magnitude, 0.f, 1.f);
        ImGui::SliderFloat("Distance with neighbors", &Boid::distance_max, 0.f, 1.f);
        ImGui::SliderFloat("Speed", &Boid::speed_factor, 0.f, 2.f);
        ImGui::SliderInt("Number of boids", &boid_number, 5, 200); // à faire marcher
        ImGui::End();

        for (auto& boid : boids)
        {
            boid.update_direction(boids);
            boid.update_velocity();
            boid.update_position(ctx.delta_time(), ctx.aspect_ratio());

            ctx.fill = {p6::random::number(0.5, 1), p6::random::number(0.5, 1), p6::random::number(0.5, 1)};
            ctx.circle(p6::Center{boid.get_pos().x, boid.get_pos().y}, p6::Radius{0.01f});
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}