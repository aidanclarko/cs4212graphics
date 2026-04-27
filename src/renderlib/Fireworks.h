#pragma once
#include "Particles.h"
#include "render_helpers.h"

struct Firework {
    glm::vec4 pos;
    glm::vec4 color;
    float life_span;
    glm::vec4 velocity;
    glm::vec4 gravity;
    float index;
    float expTime;
    float fireworkTimer;
};


class Fireworks : public Particles<Firework> {
public:
    Fireworks(int count, glm::vec4 color, glm::vec4 startingPos, glm::vec4 gravity, float lifeSpan, float expTime, float timer, float maxSize)
        : Particles(count), color(color), startingPos(startingPos), fireworkTimer(timer), maxSize(maxSize),
          gravity(gravity), lifeSpan(lifeSpan), expTime(expTime) 
          {
                particleCount = count;
                particles = std::vector<Firework>(count);
          }

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

    void setupAttributes() override {
        glGenVertexArrays(2, vao);
            for(int i = 0; i < 2; i++) {
                glBindVertexArray(vao[i]);
                glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, pos));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, color));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, life_span));
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, velocity));
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, gravity));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, index));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, expTime));
                glEnableVertexAttribArray(7);
                glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Firework), (void*)offsetof(Firework, fireworkTimer));
            }

            glBindVertexArray(0);
    }

    void initDrawShader(sivelab::GLSLObject& shader) override {
        shader.addShader( "vertexShader_particles.glsl", sivelab::GLSLObject::VERTEX_SHADER );
        shader.addShader( "fragmentShader_particles.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
        shader.createProgram();
    }

    void initUpdateShader(sivelab::GLSLObject& updateShader) override {
        updateShader.addShader("vertexShader_update_fireworks.glsl", sivelab::GLSLObject::VERTEX_SHADER);
        updateShader.addShader("fragmentShader_discard.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
        GLuint updateProgram = updateShader.createProgram();

        const char* varyings[] = { "out_pos", "out_color", "out_life_span", "out_velocity", "out_gravity", "out_index", "out_expTime", "out_fireworkTimer" };
        glTransformFeedbackVaryings(updateProgram, 8, varyings, GL_INTERLEAVED_ATTRIBS);
        glLinkProgram(updateProgram); 
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