#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "png++/png.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../OpenGL/GLSL.h"

struct Particle {
    glm::vec4 pos;
    glm::vec4 color;
    float life_span;
    glm::vec4 velocity;
    glm::vec4 gravity;
    float index;
    float expTime;
    float fireworkTimer;
};

/*
    this is where the scenes are generated, not sure how to abstract it yet...
*/

class Particles {
    public:
        Particles(int count) : particleCount(count) { particles = std::vector<Particle>(count); }
        virtual void initParticles() = 0;
        
        void initBuffers() {
            glGenBuffers(2, vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_COPY);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_COPY);

            glGenVertexArrays(2, vao);
            for(int i = 0; i < 2; i++) {
                glBindVertexArray(vao[i]);
                glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, pos));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, life_span));
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, gravity));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, index));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, expTime));
                glEnableVertexAttribArray(7);
                glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, fireworkTimer));
            }

            glBindVertexArray(0);

            glGenTransformFeedbacks(2, tfo);
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo[0]);
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[1]);
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo[1]);
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[0]);
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        }

        virtual void update(int current) {
            glBindVertexArray(vao[current]);
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo[current]);
            glBeginTransformFeedback(GL_POINTS);
            glDrawArrays(GL_POINTS, 0, particles.size());
            glEndTransformFeedback();
        }

        virtual void draw(int next) {
            glBindVertexArray(vao[next]);
            glDrawArrays(GL_POINTS, 0, particles.size());
        }

    protected:
        int particleCount;
        std::vector<Particle> particles;
        GLuint vbo[2], vao[2], tfo[2];
};