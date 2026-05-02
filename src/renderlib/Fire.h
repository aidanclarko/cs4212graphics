#pragma once
#include "Particles.h"
#include "vec3.h"
#include "render_helpers.h"

struct Fire {
    glm::vec4 pos;
    glm::vec4 color;
    float life_span;
    glm::vec4 velocity;
    glm::vec4 gravity;
    float index;
    glm::vec4 prevPos;
    float initLifeSpan;
};


class Fires : public Particles<Fire> {
public:
    
    Fires(glm::vec4 startingPos, int particleCount, glm::vec4 color, glm::vec4 gravity, float lifeSpan, float pointSize)
    : Particles(particleCount), color(color), gravity(gravity), lifeSpan(lifeSpan), pointSize(pointSize), particleCount(particleCount), startingPos(startingPos) {
        particles = std::vector<Fire>(particleCount);
    }

   void initParticles() override {
        for(int i = 0; i < particleCount; i++) {
            float ls = random_float(0.01, lifeSpan);

            float rx = random_float(-0.5, 0.25);
            float ry = random_float(-0.5, 0.25);
            float rz = random_float(-0.5, 0.25);

            particles[i].pos = glm::vec4(
                startingPos.x + rx,
                startingPos.y + ry,
                startingPos.z + rz, 1.0
            );
            particles[i].prevPos = particles[i].pos;
            particles[i].color = color;
            particles[i].velocity = glm::vec4(0.5, -0.001, 20, 0);
            particles[i].gravity = gravity;
            particles[i].life_span = ls;
            particles[i].initLifeSpan = ls;
            particles[i].index = float(i);
        }
    }

    void setupAttributes() override {
        glGenVertexArrays(2, vao);
        for(int i = 0; i < 2; i++) {
            glBindVertexArray(vao[i]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, pos));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, color));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, life_span));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, velocity));
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, gravity));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, index));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, prevPos));
            glEnableVertexAttribArray(7);
            glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Fire), (void*)offsetof(Fire, initLifeSpan));
        }
        glBindVertexArray(0);
    }

    void initDrawShader(sivelab::GLSLObject& shader) override {
        shader.addShader( "vertexShader_particles_obj.glsl", sivelab::GLSLObject::VERTEX_SHADER );
        shader.addShader( "fragmentShader_particles_obj.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
        shader.createProgram();
        
        //uniforms
        shader.activate();
        GLint pointSizeID = shader.createUniform("pointSize");
        glUniform1f(pointSizeID, pointSize);
        shader.deactivate();
    }

    void initUpdateShader(sivelab::GLSLObject& updateShader) override {
        updateShader.addShader("vertexShader_update_test.glsl", sivelab::GLSLObject::VERTEX_SHADER);
        updateShader.addShader("fragmentShader_discard.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
        GLuint updateProgram = updateShader.createProgram();

        const char* varyings[] = { "out_pos", "out_color", "out_life_span", "out_velocity", "out_gravity", "out_index", "out_prevPos", "out_initLifeSpan" };
        glTransformFeedbackVaryings(updateProgram, 8, varyings, GL_INTERLEAVED_ATTRIBS);
        
        glLinkProgram(updateProgram);

        glLinkProgram(updateProgram);
    }

private:
    std::string fileName;
    glm::vec4 color;
    glm::vec4 gravity;
    float lifeSpan;
    float pointSize;
    std::vector<vec3> vertices;
    int particleCount;
    glm::vec4 startingPos;
};