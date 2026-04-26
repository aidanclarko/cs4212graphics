#pragma once
#include "Particles.h"
#include "render_helpers.h"


class Firework : public Particles {
public:
    Firework(int count, glm::vec4 color, glm::vec4 startingPos, glm::vec4 gravity, float lifeSpan, float expTime, float timer, float maxSize)
        : Particles(count), color(color), startingPos(startingPos), fireworkTimer(timer), maxSize(maxSize),
          gravity(gravity), lifeSpan(lifeSpan), expTime(expTime) {}

    void initParticles() override {
        for(int i = 0; i < particleCount; i++) {
            float phi = random_float(0.0, 2 * M_PI);
            float theta = random_float(0.0, M_PI);
            float size = random_float(0.001, 0.02);

            particles[i].pos = startingPos;
            particles[i].color = color;
            particles[i].velocity = glm::vec4(
                size * sin(theta) * cos(phi),
                size * sin(theta) * sin(phi),
                size * cos(theta),
                0.0f
            );
            particles[i].gravity   = gravity;
            particles[i].life_span = random_float(1, lifeSpan);
            particles[i].index     = float(i);
            particles[i].expTime   = expTime;
            particles[i].fireworkTimer = fireworkTimer; 
        }
    }

private:
    float maxSize;
    float fireworkTimer;
    glm::vec4 color;
    glm::vec4 startingPos;
    glm::vec4 gravity;
    float lifeSpan;
    float expTime;
};