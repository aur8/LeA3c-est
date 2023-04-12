#include <stdlib.h>
#include <cstdlib>
#include <string>
#include "Boid.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

// initialisation magnitude

float Boid::cohesion_magnitude   = 0.5f;
float Boid::alignment_magnitude  = 0.5f;
float Boid::separation_magnitude = 0.5f;
float Boid::distance_max         = 0.5f;

int main()
{
    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // { // Run the tests
    //     if (doctest::Context{}.run() != 0)
    //         return EXIT_FAILURE;
    //     // The CI does not have a GPU so it cannot run the rest of the code.
    //     const bool no_gpu_available =
    //         argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    //     if (no_gpu_available)
    //         return EXIT_SUCCESS;
    // }

    // Actual app
    auto ctx = p6::Context{{.title = "Papeterie"}};
    ctx.maximize_window();

    std::vector<Boid> boids(100);

    // initialisation des positions de boid
    for (auto& boid : boids)
    {
        boid.set_pos(
            glm::vec2(p6::random::number(-2, 2), p6::random::number(-1, 1))
        );
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}