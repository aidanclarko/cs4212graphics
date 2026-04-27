#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "png++/png.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../OpenGL/GLSL.h"

template<typename T>
class Particles {
    public:
        Particles(int count) : particleCount(count) {}
        virtual void initParticles() = 0;
        virtual void setupAttributes() = 0;
        virtual void initUpdateShader(sivelab::GLSLObject& updateShader) = 0;
        virtual void initDrawShader(sivelab::GLSLObject& shader) = 0;
        
        void initBuffers() {
            glGenBuffers(2, vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(T), particles.data(), GL_DYNAMIC_COPY);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(T), particles.data(), GL_DYNAMIC_COPY);

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
        std::vector<T> particles;
        GLuint vbo[2], vao[2], tfo[2];
};