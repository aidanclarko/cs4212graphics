#pragma once
#include "Particles.h"
#include "render_helpers.h"


// no need for transform feedback since they are static


struct Star {
    glm::vec4 pos;
    glm::vec4 color;
};

class Stars : public Particles<Star> {
public:
    Stars(int count, glm::vec4 color, float pointSize)
        : Particles(count), color(color), pointSize(pointSize)
          {
                particleCount = count;
                particles = std::vector<Star>(count);
          }

    void initParticles() override {
        for(int i = 0; i < particleCount; i++) {
           particles[i].pos = glm::vec4(
                random_float(-200, 200),
                random_float(20, 150),
                random_float(-200, 200),
                1
            );
            particles[i].color = glm::vec4(1, 1, 1, 1);
        }
    }

    void setupAttributes() override {
        glGenVertexArrays(2, vao);
            for(int i = 0; i < 2; i++) {
                glBindVertexArray(vao[i]);
                glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, pos));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, color));
            }

            glBindVertexArray(0);
    }

    void initDrawShader(sivelab::GLSLObject& shader) override {
        shader.addShader( "vertexShader_particles_stars.glsl", sivelab::GLSLObject::VERTEX_SHADER );
        shader.addShader( "fragmentShader_particles_stars.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );

        shader.createProgram();

        //uniforms
        shader.activate();
        GLint pointSizeID = shader.createUniform("pointSize");
        glUniform1f(pointSizeID, pointSize);
        shader.deactivate();
    }

    void initUpdateShader(sivelab::GLSLObject& updateShader) override {
        updateShader.addShader("vertexShader_passthrough.glsl", sivelab::GLSLObject::VERTEX_SHADER);
        updateShader.addShader("fragmentShader_discard.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
        GLuint updateProgram = updateShader.createProgram();

        const char* varyings[] = { "out_pos", "out_color" };
        glTransformFeedbackVaryings(updateProgram, 2, varyings, GL_INTERLEAVED_ATTRIBS);
        glLinkProgram(updateProgram); 
    }

private:
    float pointSize;
    glm::vec4 color;
};