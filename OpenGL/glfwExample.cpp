#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "png++/png.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"

#include "vec3.h"
#include "PerspectiveCamera.h"
#include "Triangle.h"
#include "ObjMesh.h"
#include "GLMSphere.h"
#include "render_helpers.h"
#include "Fireworks.h"

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        exit (-1);
    }
    // throw std::runtime_error("Error! initialization of glfw failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    int winWidth = 1000;
    float aspectRatio = 1.0; // 16.0 / 9.0; // winWidth / (float)winHeight;
    int winHeight = winWidth / aspectRatio;
    
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err=glewInit();
    if(err != GLEW_OK) {
        std::cerr <<"GLEW Error! glewInit failed, exiting."<< std::endl;
        exit(EXIT_FAILURE);
    }

    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0, 0, 0, 1.0);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    // Need to set a projection matrix that fits the aspect ratio set
    // by the window frame.
    //
    // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
    float halfWidth = 15.0 / 2.0;
    float halfHeight = halfWidth;

    float left = -halfWidth;
    float right = halfWidth;

    float bottom = -halfHeight;
    float top = halfHeight;

    float near = 5.0f;
    float far = -5.0f;

    // glm::mat4 M_ortho = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
    vec3 m_pos(0, 0, 3);  
    vec3 m_viewDir(0, 0, -1);

    PerspectiveCamera cam(fb_width, fb_height, m_pos, m_viewDir, 0.5f, 1.0f, 3.14159f/4.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 M_pers = cam.getPerspectiveMatrix();


    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Firework f1(
        10000,
        glm::vec4(1.0, 0.0, 0.0, 0.0),
        glm::vec4(0, 0, -10, 1),
        glm::vec4(0.0, -0.00001, 0.0, 0.0),
        5, 3, 3, 0.01
    );

    Firework f2(
        1000000,
        glm::vec4(0.0, 1.0, 0.0, 0.0),
        glm::vec4(-5, 0, -20, 1),
        glm::vec4(0.0, -0.000002, 0.0, 0.0),
        4, 3, 5, 0.01
    );

    Firework f3(
        1000,
        glm::vec4(0.0, 1.0, 1.0, 0.0),
        glm::vec4(4, 0, -5, 1),
        glm::vec4(0.0, -0.00025, 0.0, 0.0),
        5, 3, 0, 0.002
    );

    f1.initParticles();
    f2.initParticles();
    f3.initParticles();
    f1.initBuffers();
    f2.initBuffers();
    f3.initBuffers();



    // Create a shader using my GLSLObject class                                                            
    sivelab::GLSLObject shader, updateShader;
    
    shader.addShader( "vertexShader_particles.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "fragmentShader_particles.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    shader.createProgram();
    

    updateShader.addShader("vertexShader_update.glsl", sivelab::GLSLObject::VERTEX_SHADER);
    updateShader.addShader("fragmentShader_discard.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
    GLuint updateProgram = updateShader.createProgram();

    const char* varyings[] = { "out_pos", "out_color", "out_life_span", "out_velocity", "out_gravity", "out_index", "out_expTime", "out_fireworkTimer" };
    glTransformFeedbackVaryings(updateProgram, 8, varyings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(updateProgram); 
    

    GLuint projMatrixID, viewMatrixID, modelMatrixID, normalMatrixID, lightPosID, diffID, camPosID, specularID, shininessID, texUnitID;
    projMatrixID = shader.createUniform( "projMatrix" );
    viewMatrixID = shader.createUniform( "viewMatrix" );
    modelMatrixID = shader.createUniform( "modelMatrix" );
    normalMatrixID = shader.createUniform( "normalMatrix" );
    lightPosID = shader.createUniform( "lightPos" );
    camPosID = shader.createUniform( "camPos" );
    specularID = shader.createUniform( "specular" );
    shininessID = shader.createUniform( "shininess" );
    diffID = shader.createUniform( "diffuse" );
    texUnitID = shader.createUniform( "texUnit" );


    glm::mat4 modelTransform = glm::mat4(1.0);

    float rot  = 0.0f;
    // modelTransform = glm::rotate(modelTransform, rot, glm::vec3(0, 1, 0));


    glm::mat4 M_normal = glm::mat4(1.0);


    glm::vec3 lightPos(-6, 2, 20);
    

    //Shader Components
    glm::vec3 diffuseSph(0.263, 0.678, 0.306);
    glm::vec3 diffuseSphTwo(0.812, 0.388, 0.706);
    glm::vec3 diffuseTriOne(0.196, 0.98, 0.616);
    glm::vec3 diffuseTriTwo(0.329, 0.988, 1);
    glm::vec3 diffuseTriThree(0.486, 0.176, 0.702);
    glm::vec3 diffuseCrow(0.749, 0.329, 0.329);
    glm::vec3 specular(1.0f, 1.0f, 1.0f);
    float shininess = 120.0f;
    float shininessT = 80.0f;


    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;


    
    float rotationAngle = 0.0f;
    //MESH MODE
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    
    float camAngle = 0.0f;

   int current = 0;
   glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window))
    {  
        glm::vec3 camPos = cam.getGLMPos();
        endFrameTime = glfwGetTime();
        timeDiff = endFrameTime - startFrameTime;
        startFrameTime = glfwGetTime();


        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_RASTERIZER_DISCARD);

        glm::mat4 M_view = cam.lookAt();
        M_normal = glm::transpose(glm::inverse( modelTransform ));
        /* Render your objects here */
        updateShader.activate();
        f1.update(current);
        f2.update(current);
        f3.update(current);

        updateShader.deactivate();
        glDisable(GL_RASTERIZER_DISCARD);
                                                                                    
        int next = 1 - current;

        shader.activate();
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(M_pers));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(M_view));
        f1.draw(next);
        f2.draw(next);
        f3.draw(next);

        glBindVertexArray(0);
        shader.deactivate();
        current = next;

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        float moveRatePerFrame = 0.05;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam.moveForward(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam.moveLeft(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam.moveBack(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam.moveRight(moveRatePerFrame);
        }

        else if(glfwGetKey(window, GLFW_KEY_LEFT)) {
            cam.rotateY(0.02f);
        }

        else if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
            cam.rotateY(-0.02f);
        }

        else if(glfwGetKey(window, GLFW_KEY_UP)) {
            cam.rotateX(0.02f);
        }

        else if(glfwGetKey(window, GLFW_KEY_DOWN)) {
            cam.rotateX(-0.02f);
        }

        if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS) {
            std::cout << "fps: " << 1.0/timeDiff << std::endl;
        }
        if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

    }
  
    glfwTerminate();
    return 0;
}
