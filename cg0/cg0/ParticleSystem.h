#pragma once
#include "Particle.h"
#include "CommonHeader.h"
#include <vector>
#include <iterator>

#define LIFE_BASE 2.5
#define LIFE_VAR  0.1
#define SIZE_BASE 0.03
#define randUN rand() / (float)RAND_MAX
#define randSN (1 - 2 * randUN)
#define PI 3.1415926

class ParticleSystem {
public:
    ParticleSystem();
    ParticleSystem(int maxParticles, glm::vec3 origin);
    virtual void update(float deltaT);
    virtual void render();
    virtual void emit(int numParticles);
    virtual void initializeSystem();
private:
    std::vector<Particle> particles;       // particles for this emitter
protected:
    int                   maxParticles;    // maximum number of particles in total
    glm::vec3             origin;          // center of the particle system
    glm::vec3             force;           // force (gravity, wind, etc.) acting on the system
};
