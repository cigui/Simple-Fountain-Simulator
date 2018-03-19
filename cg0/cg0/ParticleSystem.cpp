#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
    this->maxParticles = 1000;
    this->origin = glm::vec3(0.0, 0.0, 0.0);
    this->force = glm::vec3(0, -0.98, 0);
    this->initializeSystem();
}

ParticleSystem::ParticleSystem(int maxParticles, glm::vec3 origin) {
    this->maxParticles = maxParticles;
    this->origin = origin;
    this->force = glm::vec3(0, -0.98, 0);
    this->initializeSystem();
}

void ParticleSystem::initializeSystem() {
    Particle first;
    first.postition = origin;
    first.velocity = glm::vec3(0, 1.0, 0);
    first.color = glm::vec4(0.4, 0.8, 1.0, 0.5);
    first.size = 0.01;
    particles.push_back(first);
}

void ParticleSystem::update(float deltaT) {
    float r = randUN;
    std::vector<Particle>::iterator it;
    for (it = particles.begin(); it != particles.end();) {
        Particle &p = *it;
        p.life -= deltaT;
        if (p.life > 0 && p.postition.y >= 0) {
            p.postition += p.velocity * deltaT;
            p.velocity += force * deltaT;
            p.color.a = p.life;
            it++;
        }
        else {
            /*if (p.postition.y <= 0) {
                droppedOnes.push_back(p);
            }*/
            it = particles.erase(it);
        }
    }
    int numToGen = (int)(particles.size() * r * 2);
    if (numToGen > maxParticles - particles.size()) numToGen = (int)((maxParticles - particles.size()) * r);
    emit(numToGen);
}

void ParticleSystem::emit(int num) {
    float r = randUN;
    for (int i = 0; i < num; i++) {
        Particle p;
        p.color = glm::vec4(randUN, randUN, randUN, randUN);
        p.life = LIFE_BASE + LIFE_VAR * r;
        p.postition = origin;
        p.velocity = glm::vec3(randSN * 0.3, randUN * 2.5, randSN * 0.3);
        p.size = r * SIZE_BASE;
        particles.push_back(p);
    }
}

void ParticleSystem::render() {
    std::vector<Particle>::iterator it;
    float x, y, z, r, g, b, a, size;
    for (it = particles.begin(); it != particles.end(); it++) {
        Particle &p = *it;
        x = p.postition.x;
        y = p.postition.y;
        z = p.postition.z;
        r = p.color.r;
        g = p.color.g;
        b = p.color.b;
        a = p.color.a;
        size = p.size;
        glColor4f(r, g, b, a);
        glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidSphere(size, 10, 10);
        glPopMatrix();
    }
}