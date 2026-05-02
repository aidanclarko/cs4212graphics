#pragma once
#include "Particles.h"
#include "render_helpers.h"

struct Wave {
    glm::vec4 pos;
    glm::vec4 color;
};

class ParticleWave : public Particles<Wave> {
public:
    ParticleWave(int gridSize, float spacing, float pointSize)
        : Particles(gridSize * gridSize), gridSize(gridSize), spacing(spacing), pointSize(pointSize) {
        particles = std::vector<Wave>(gridSize * gridSize);
    }

    void initParticles() override {
        int p = 0;
        for(int x = 0; x < gridSize; x++) {
            for(int z = 0; z < gridSize; z++) {
                float fx = x * spacing - (gridSize * spacing / 2.0f);
                float fz = z * spacing - (gridSize * spacing / 2.0f);
                particles[p].pos = glm::vec4(fx, 0.0f, fz, 1.0f);
                particles[p].color = glm::vec4(0, 0.5f, 1.0f, 1.0f);
                p++;
            }
        }
    }

    void setupAttributes() override {
        glGenVertexArrays(2, vao);
        for(int i = 0; i < 2; i++) {
            glBindVertexArray(vao[i]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Wave), (void*)offsetof(Wave, pos));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Wave), (void*)offsetof(Wave, color));
        }
        glBindVertexArray(0);
    }

    void initDrawShader(sivelab::GLSLObject& shader) override {
        shader.addShader("vertexShader_particles_wave.glsl", sivelab::GLSLObject::VERTEX_SHADER);
        shader.addShader("fragmentShader_particles_wave.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
        shader.createProgram();

        shader.activate();
        GLint psID = shader.createUniform("pointSize");
        glUniform1f(psID, pointSize);
        shader.deactivate();
    }

     void initUpdateShader(sivelab::GLSLObject& updateShader) override {
        updateShader.addShader("vertexShader_update_wave.glsl", sivelab::GLSLObject::VERTEX_SHADER);
        updateShader.addShader("fragmentShader_discard.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
        GLuint updateProgram = updateShader.createProgram();

        const char* varyings[] = { "out_pos", "out_color" };
        glTransformFeedbackVaryings(updateProgram, 2, varyings, GL_INTERLEAVED_ATTRIBS);
        
        glLinkProgram(updateProgram);

        glLinkProgram(updateProgram);
    }

private:
    int gridSize;
    float spacing;
    float pointSize;
};