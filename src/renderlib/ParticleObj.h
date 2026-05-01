#pragma once
#include "Particles.h"
#include "ObjMesh.h"
#include "vec3.h"
#include "render_helpers.h"

struct Obj {
    glm::vec4 pos;
    glm::vec4 color;
    float life_span;
    glm::vec4 velocity;
    glm::vec4 gravity;
    float index;
    glm::vec4 prevPos;
    float initLifeSpan;
};


class ParticleObj : public Particles<Obj> {
public:
    
    ParticleObj(std::string fileName, glm::vec4 color, glm::vec4 gravity, float lifeSpan, int density, float pointSize)
    : Particles(0), color(color), gravity(gravity), lifeSpan(lifeSpan), fileName(fileName), density(density), pointSize(pointSize) {
        vertices = loadVertices(fileName);
        particleCount = vertices.size() * density;
        std::cout << particleCount << std::endl;
        particles = std::vector<Obj>(particleCount);
    }

    std::vector<vec3> loadVertices(const std::string& fileName) {
        ObjMesh obj(fileName);
        return obj.getVerticies();
    }

   void initParticles() override {
        int p = 0;
        for(int i = 0; i < vertices.size(); i++) {
            for(int d = 0; d < density; d++) {
                float x = vertices[i].x() + random_float(-0.1f, 0.5f);
                float y = vertices[i].y() + random_float(-0.1f, 0.5f);
                float z = vertices[i].z() + random_float(-0.1f, 0.5f);
                float ls = random_float(0.01, lifeSpan);

                particles[p].prevPos = glm::vec4(x, y, z, 1.0f);
                particles[p].pos = glm::vec4(x, y, z, 1);
                particles[p].color = color;
                particles[p].velocity = glm::vec4(0.5, -0.01, 50.0, 0);
                particles[p].gravity = gravity;
                particles[p].life_span = ls;
                particles[p].initLifeSpan = ls;
                particles[p].index = float(p);
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
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, pos));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, color));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, life_span));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, velocity));
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, gravity));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, index));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, prevPos));
            glEnableVertexAttribArray(7);
            glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Obj), (void*)offsetof(Obj, initLifeSpan));
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
    int density;
    std::string fileName;
    glm::vec4 color;
    glm::vec4 gravity;
    float lifeSpan;
    float pointSize;
    std::vector<vec3> vertices;
};