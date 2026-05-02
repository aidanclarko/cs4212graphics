#pragma once
#include "vec3.h"
#include "Particles.h"
#include "render_helpers.h"

// no need for transform feedback since they are static

struct Tree {
    glm::vec4 pos;
};

class Trees : public Particles<Tree> {
public:
    Trees(ObjMesh& o, float pointSize, int count) : count(count), pointSize(pointSize), o(o), Particles(0)
    {
        particleCount = count;
        particles = std::vector<Tree>(count);
    }

    void initParticles() override {
        float offset = 0.5;
        std::vector< vec3 > terrainPos = o.getVerticies();
        for(int i = 0; i < count; i++) {
            vec3 pos = terrainPos[random_float(0, terrainPos.size() - 1 )];
            particles[i].pos = glm::vec4(pos.x() - offset, pos.y() - offset, pos.z() - offset, 1);
        }
    }

    void initDrawShader(sivelab::GLSLObject& shader) override {
        shader.addShader("vertexShader_tree.glsl", sivelab::GLSLObject::VERTEX_SHADER);
        shader.addShader("fragmentShader_tree.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
        shader.createProgram();

        shader.activate();
        glUniform1f(shader.createUniform("pointSize"), pointSize);
        shader.deactivate();
    }

    void setupAttributes() override {
        float offsets[] = {
            -1, 0,   1, 0,   1, 2,
            -1, 0,   1, 2,  -1, 2
        };
        float uvs[] = {
            0, 0,   1, 0,   1, 1,
            0, 0,   1, 1,   0, 1
        };

        glGenVertexArrays(2, vao);
        glGenBuffers(1, &quadOffsetVBO);
        glGenBuffers(1, &quadUVVBO);

        for(int i = 0; i < 2; i++) {
            glBindVertexArray(vao[i]);

            glBindBuffer(GL_ARRAY_BUFFER, quadOffsetVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glBindBuffer(GL_ARRAY_BUFFER, quadUVVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Tree), (void*)offsetof(Tree, pos));
            glVertexAttribDivisor(2, 1);
        }
        glBindVertexArray(0);
    }

    void initUpdateShader(sivelab::GLSLObject& updateShader) override {
        // static just needs a passthrough
        updateShader.addShader("vertexShader_passthrough.glsl", sivelab::GLSLObject::VERTEX_SHADER);
        updateShader.addShader("fragmentShader_discard.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
        GLuint updateProgam = updateShader.createProgram();
        const char* varyings[] = { "out_pos" };
        glTransformFeedbackVaryings(updateProgam, 1, varyings, GL_INTERLEAVED_ATTRIBS);
        glLinkProgram(updateProgam);
    }

    void draw(int next) override {
        glBindVertexArray(vao[next]);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleCount);
        glBindVertexArray(0);
    }



private:
    float pointSize;
    int count;
    ObjMesh o;
    GLuint quadOffsetVBO, quadUVVBO;
};