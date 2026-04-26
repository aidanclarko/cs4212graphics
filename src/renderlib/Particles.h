#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "png++/png.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"

struct Particle {
    glm::vec4 pos;
    glm::vec4 color;
    float life_span;
    glm::vec4 velocity;
    glm::vec4 gravity;
};

/*
    this is where the scenes are generated, not sure how to abstract it yet...
*/

class Particles {
    public:
        Particles(int count) : particleCount(count) {}
        virtual void initParticles() = 0;
        virtual void particleGenBuffers(GLuint vbo, GLuint vao) = 0;
        virtual void particleGenTransformFeedback(GLuint tfo) = 0;
        



    private:
        int particleCount;
        std::vector<Particle> particles;
};