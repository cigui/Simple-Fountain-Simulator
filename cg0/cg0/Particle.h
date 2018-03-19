#pragma once
#include <glm/glm.hpp>

typedef struct Particle {

    Particle()
        : postition(0)
        , velocity(0)
        , color(0)
        , life(5)
    {}

    glm::vec3   postition; // Center point of particle
    glm::vec3   velocity;  // Current particle velocity
    glm::vec4   color;     // Particle color
    float life;            // Life
    float size;            // Size
} Particle;
